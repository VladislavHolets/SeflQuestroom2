/*
 * questserver.cpp
 *
 *  Created on: May 16, 2021
 *      Author: vladi
 */

#include <ArduinoJson.h>
#include <utils/logger.h>
#include <mqtt_wrappers/mqtt.h>
#include <quest_clients/questboardmanager.h>
#include <stddef.h>
#include <Vector.h>
#include <wiring_constants.h>
#include <wiring_digital.h>
#include <wiring_time.h>
#include <WString.h>
#include <cstring>

namespace SEFL
{

	Quest_Board_Manager::Quest_Board_Manager(MQTT &mqtt, const char *host_name,
											 const char *placement, const char *server_name, const char *in_topic,
											 const char *out_topic, SEFL::Language language) : MQTT_Two_Way_Interactor(mqtt,
																													   String(
																														   String(placement) + TOPIC_DELIMETER + server_name + TOPIC_DELIMETER + in_topic),
																													   String(
																														   String(placement) + TOPIC_DELIMETER + server_name + TOPIC_DELIMETER + out_topic)),
																							   host_(mqtt,
																									 host_name, placement, out_topic, in_topic, language),
																							   clients_(), shutdown_timestamp(
																											   0),
																							   callback_timestamp(0), name_(server_name), language_(
																																			  language),
																							   power_status_(true)
	{
		this->clients_.setStorage(clients_storage, MAX_CLIENT_AMOUNT);
		this->clients_.clear();
		this->callbacksQueue.setStorage(callbackQueue_storage, MAX_MESSAGE_QUEUE_AMOUNT);
		this->callbacksQueue.clear();
		Logger::notice("board_manager", F("constructor_done"));
	}

	Quest_Board_Manager::~Quest_Board_Manager()
	{
	}

	Quest_Host_Client *Quest_Board_Manager::getHost()
	{
		return (&host_);
	}

	Language Quest_Board_Manager::getLanguage() const
	{
		return (language_);
	}

	void Quest_Board_Manager::setLanguage(Language language_)
	{
		this->language_ = language_;
	}

	const char *Quest_Board_Manager::getName() const
	{
		return (name_);
	}

	void Quest_Board_Manager::send_config()
	{
		DynamicJsonDocument repDoc(JSON_ARRAY_SIZE(33) + JSON_OBJECT_SIZE(2) + 40);
		repDoc["CommandId"] = static_cast<int>(REQUEST_CONFIG_COMMAND);
		JsonArray data = repDoc.createNestedArray("Data");
		data.add(this->host_.getName());
		data.add(String(static_cast<unsigned int>(this->host_.getType())));
		for (unsigned int i = 0; i < this->clients_.size(); i++)
		{
			if (this->clients_[i] != nullptr)
			{
				data.add(this->clients_[i]->getName());
				data.add(String(static_cast<unsigned int>(this->clients_[i]->getType())));
			}
		}
		String output;
		serializeJson(repDoc, output);
		this->getMqtt()->publish(this->getPubfeed().c_str(), output.c_str(), 1);
	}

	void Quest_Board_Manager::removeAllClients()
	{
		this->clients_.clear();
	}

	void Quest_Board_Manager::removeClient(SEFL::Quest_Client *client)
	{
		for (unsigned int i = 0; i < this->clients_.size(); i++)
		{
			if (this->clients_[i] == client)
			{
				this->clients_.remove(i);
			}
		}
	}

	void Quest_Board_Manager::loop()
	{
		if ((millis() - this->callback_timestamp) > SEFL::CALLBACK_TIMEOUT)
		{
			if (!this->getMqtt()->ping())
			{
				this->getMqtt()->disconnect();
			}
		}

		// Stop if already connected.
		if (!this->getMqtt()->connected())
		{
			int8_t ret;
			Logger::notice(this->name_, "Connecting to MQTT... ");
			while ((ret = this->getMqtt()->connect()) != 0)
			{ // connect will return 0 for connected
				SEFL::Logger::error(this->name_,
									this->getMqtt()->connectErrorString(ret));
				Logger::notice(this->name_,
							   "Retrying MQTT connection in 1 second...");
				this->getMqtt()->disconnect();
				delay(2000); // wait 1 seconds
			}
			Logger::notice(this->name_, "MQTT Connected!");
			this->send_config();
			this->callback_timestamp = millis();
		}
		if (this->getMqtt()->processPackets(100))
		{
			Logger::notice(this->name_, "packets received");
			this->callback_timestamp = millis();
		}
		if (!this->power_status_ && this->shutdown_timestamp && (millis() - this->shutdown_timestamp) > SEFL::shutdown_timeout)
		{

			Logger::notice(this->name_, "shut down");
			//		for (int i = 0; i < 8; i++) {
			//			digitalWrite(6 + i, LOW);
			//		}
			this->shutdown_timestamp = 0;
		}
		if (this->power_status_ && this->shutdown_timestamp)
		{

			Logger::notice(this->name_, "clients initial stuff");
			for (unsigned int i = 0; i < this->clients_.size(); i++)
			{
				if (this->clients_[i] != nullptr)
				{
					this->clients_[i]->setChangedStatus(true);
				}
			}
			this->shutdown_timestamp = 0;
		}
		if (this->power_status_ && !this->shutdown_timestamp)
		{

			Logger::notice(this->name_, "clients stuff");
			for (unsigned int i = 0; i < this->clients_.size(); i++)
			{
				if (this->clients_[i] != nullptr)
				{
					this->clients_[i]->act();
				}
			}
		}
		if (this->host_.reset_trigger_)
		{
			this->host_.reset_trigger_ = false;
			for (unsigned int i = 0; i < this->clients_.size(); i++)
			{
				if (this->clients_[i] != nullptr)
				{
					this->clients_[i]->setStatus(this->clients_[i]->reset_status_);
					this->clients_[i]->cleanData();
				}
			}
		}
	}

	bool Quest_Board_Manager::addClient(SEFL::Quest_Client *client)
	{

		for (unsigned int i = 0; i < this->clients_.size(); i++)
		{
			if (this->clients_[i] == client)
			{
				return (1);
			}
		}
		this->clients_.push_back(client);
		return (0);
	}

	bool Quest_Board_Manager::isPowerStatus() const
	{
		return (power_status_);
	}

	void Quest_Board_Manager::setPowerStatus(bool power_status_)
	{
		this->power_status_ = power_status_;
	}

	void Quest_Board_Manager::inputClb(const char *data, uint16_t len)
	{
		//  { "CommandId": 0,
		//    "Arguments": []
		//  }

		const size_t capacity = JSON_ARRAY_SIZE(0) + JSON_OBJECT_SIZE(2) + 40;
		DynamicJsonDocument doc(capacity);
		deserializeJson(doc, data, len);
		SEFL::BroadcastComands command =
			static_cast<SEFL::BroadcastComands>(doc["CommandId"].as<uint8_t>());
		switch (command)
		{

		case SEFL::BroadcastComands::REQUEST_CONFIG_COMMAND:
		{
			send_config();
		}
		break;
		case SEFL::BroadcastComands::LOCALIZATION_SETUP_COMMAND:
		{
			const char *lang = doc["Data"][0];
			if (!strcmp(lang, "ENG"))
			{
				this->language_ = SEFL::Language::ENG;
			}
			else if (!strcmp(lang, "UKR"))
			{
				this->language_ = SEFL::Language::UKR;
			}
			else if (!strcmp(lang, "RUS"))
			{
				this->language_ = SEFL::Language::RUS;
			}
			for (unsigned int i = 0; i < this->clients_.size(); i++)
			{
				if (this->clients_[i] != nullptr)
				{
					this->clients_[i]->setLanguage(this->language_);
					// SEFL::Logger::verbose(clients_[i]->getName());
					// SEFL::Logger::verbose(clients_[i]->getLanguage());
				}
			}
		}
		break;
		case SEFL::BroadcastComands::ENABLE_POWER_COMMAND:
		{

			this->setPowerStatus(true);
			this->shutdown_timestamp = 1;
		}
		break;
		case SEFL::BroadcastComands::DISABLE_POWER_COMMAND:
		{
			this->setPowerStatus(false);
			this->shutdown_timestamp = millis();
		}
		break;
		default:
		{
		}
		}
	}
	bool Quest_Board_Manager::checkSubscribitions(String &topic_name, String &payload_val)
	{
		if (topic_name.equals(this->getSubfeed()))
		{
			this->pushToCallbacksQueue(*this, payload_val);
		}
		if (topic_name.equals(this->getHost()->getSubfeed()))
		{
			this->pushToCallbacksQueue(*(this->getHost()), payload_val);
		}
		for (auto client : this->clients_)
		{
			if (topic_name
					.equals(client->getSubfeed()))
				this->pushToCallbacksQueue(*client, payload_val);
		}
	}

	bool Quest_Board_Manager::pushToCallbacksQueue(MQTT_Two_Way_Interactor &client, String &payload_val)
	{
		CallbackItem *item = new CallbackItem();
		item->client = &client;
		item->payload = new String(payload_val);
		this->callbacksQueue.push_back(item);
	}
	void Quest_Board_Manager::processCallbackQueueOne()
	{
		this->callbacksQueue[0]->client->inputClb(this->callbacksQueue[0]->payload->c_str(), this->callbacksQueue[0]->payload->length());
		delete this->callbacksQueue[0];
		this->callbacksQueue.remove(0);
	}
	void Quest_Board_Manager::processCallbackQueueAll()
	{
		while (callbacksQueue.size())
		{
			processCallbackQueueOne();
		}
	}
	void Quest_Board_Manager::processCallbackQueue(int number)
	{
		for (size_t i = 0; i < number && callbacksQueue.size(); i++)
		{
			processCallbackQueueOne();
		}
	}
} /* namespace SEFL */

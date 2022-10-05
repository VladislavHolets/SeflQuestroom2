/*
 * questserver.cpp
 *
 *  Created on: May 16, 2021
 *      Author: vladi
 */

#include <ArduinoJson.h>
#include "../utils/logger.h"
#include "questboardmanager.h"
#include <stddef.h>
#include <Vector.h>
#include <wiring_constants.h>
#include <wiring_digital.h>
#include <wiring_time.h>
#include <WString.h>
#include <cstring>

namespace SEFL
{

	Quest_Board_Manager::Quest_Board_Manager(MQTTClientObjectBound<Quest_Board_Manager> &mqtt, MQTT_Config room_config, const char *host_name,
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
																							   power_status_(true), room_config_(room_config)
	{

		this->clients_.setStorage(clients_storage, MAX_CLIENT_AMOUNT);
		this->clients_.clear();
		this->callbacksQueue.setStorage(callbackQueue_storage, MAX_MESSAGE_QUEUE_AMOUNT);
		this->callbacksQueue.clear();
		mqtt.onMessage(this, &SEFL::Quest_Board_Manager::pushToCallbacksQueue);
		Logger::notice("board_manager", F("constructor_done"));
		SEFL::clbwrapobj = &mqtt;
	}

	Quest_Board_Manager::~Quest_Board_Manager()
	{
	}

	Quest_Host_Client *Quest_Board_Manager::getHost()
	{
		return (&host_);
	}

	Language Quest_Board_Manager::getLanguage()
	{
		return (language_);
	}

	void Quest_Board_Manager::setLanguage(Language language_)
	{
		this->language_ = language_;
	}

	const char *Quest_Board_Manager::getName()
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
		publish(output);
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
		if (!this->getMqtt()->connected())
		{
			connect();
		}
		uint32_t timestamp_for_message_avaiting = millis();
		while (millis() - timestamp_for_message_avaiting < 100)
		{
			this->getMqtt()->loop();
			if (!this->getMqtt()->connected())
			{
				connect();
			}
		}
		this->processCallbackQueueAll();
		if (!this->power_status_ && this->shutdown_timestamp && (millis() - this->shutdown_timestamp) > SEFL::shutdown_timeout)
		{
			Logger::notice(this->name_, "shut down");
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
			Logger::notice(this->name_, "clients done");
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

	bool Quest_Board_Manager::isPowerStatus()
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

	void Quest_Board_Manager::pushToCallbacksQueue(String &topic_name, String &payload_val)
	{
		CallbackItem *item = new CallbackItem();
		item->subfeed = new String(topic_name);
		item->payload = new String(payload_val);
		Logger::notice(this->name_, "pushed callback ");
		Logger::notice(this->name_, item->subfeed->c_str());
		Logger::notice(this->name_, item->payload->c_str());
		this->callbacksQueue.push_back(item);
	}
	void Quest_Board_Manager::processCallbackQueueOne()
	{
		if (this->callbacksQueue.size() == 0)
			return;
		Logger::notice(this->name_, callbacksQueue[0]->subfeed->c_str());

		Logger::notice(this->name_, "started");
		if (this->callbacksQueue[0]->subfeed->equals(this->getSubfeed()))
		{
			this->inputClb(this->callbacksQueue[0]->payload->c_str(), this->callbacksQueue[0]->payload->length());
		}
		else if (this->callbacksQueue[0]->subfeed->equals(this->getHost()->getSubfeed()))
		{
			this->getHost()->inputClb(this->callbacksQueue[0]->payload->c_str(), this->callbacksQueue[0]->payload->length());
		}
		else
		{
			for (auto client : this->clients_)
			{
				if (this->callbacksQueue[0]->subfeed->equals(client->getSubfeed()))
				{
					client->inputClb(this->callbacksQueue[0]->payload->c_str(), this->callbacksQueue[0]->payload->length());
				}
			}
		}

		Logger::notice(this->name_, "finished");
		delete this->callbacksQueue[0]->subfeed;
		delete this->callbacksQueue[0]->payload;
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
	bool Quest_Board_Manager::connect()
	{
		while (!this->getMqtt()->connected())
		{
			Logger::notice(this->name_, F("Connecting to MQTT... "));
			while (this->getMqtt()->connect(room_config_.IP, room_config_.username, room_config_.password) == 0)
			{
				Logger::notice(this->name_,
							   F("Retrying MQTT connection in 1 second..."));
				this->getMqtt()->disconnect();
				delay(1000);
			}

			Logger::notice(this->name_, F("MQTT Connected!"));
			Logger::notice(this->name_, F("Subscribing to the topics..."));

			subscribeAll();

			send_config();
			this->callback_timestamp = millis();
			return 0;
		}
		return 0;
	}

	bool Quest_Board_Manager::subscribeAll()
	{
		bool ret = 1;
		ret = ret && this->getMqtt()->subscribe(this->getSubfeed(), SEFL::QOS_DEFAULT);
		Logger::warning(this->name_, String("Subscribing to server: ") + (ret) ? F("success") : F("failure"));
		if (!ret)
		{
			return (ret);
		}
		ret = ret && this->getMqtt()->subscribe(this->getHost()->getSubfeed(), SEFL::QOS_DEFAULT);
		Logger::warning(this->name_, String("Subscribing host: ") + (ret) ? F("success") : F("failure"));
		if (!ret)
		{
			return (ret);
		}
		for (auto client : this->clients_)
		{
			ret = ret && this->getMqtt()->subscribe(client->getSubfeed(), SEFL::QOS_DEFAULT);
			Logger::warning(this->name_, String("Subscribing client ") + client->getName() + F(": ") + (ret) ? F("success") : F("failure"));
			if (!ret)
			{
				return (ret);
			}
		}
		return !ret;
	}

} /* namespace SEFL */

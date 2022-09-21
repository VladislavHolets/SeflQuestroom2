/*
 * questbasicclient.cpp
 *
 *  Created on: May 16, 2021
 *      Author: vladi
 */

#include <ArduinoJson.h>
#include <Array.h>
#include <mqtt_wrappers/mqtt.h>
#include <quest_clients/questbasicclient.h>
#include <WString.h>

namespace SEFL
{

	Quest_Basic_Client::~Quest_Basic_Client()
	{
		// TODO Auto-generated destructor stub
	}

	void Quest_Basic_Client::act()
	{
		switch (this->getStatus())
		{
		case SEFL::BasicClientStatuses::DEFAULT_STATUS:
		{
			this->onDefault();
		}
		break;
		case SEFL::BasicClientStatuses::ACTIVE_STATUS:
		{
			this->onActive();
		}
		break;
		case SEFL::BasicClientStatuses::FINISHED_STATUS:
		{
			this->onFinished();
		}
		break;
		case SEFL::BasicClientStatuses::MANUAL_FINISHED_STATUS:
		{
			this->onManualFinished();
		}
		break;
		}
	}

	void Quest_Basic_Client::inputClb(const char *data, uint16_t len)
	{
		DynamicJsonDocument doc(SEFL::DOC_SIZE);
		deserializeJson(doc, data, len);
		JsonArray arr = doc["Data"].as<JsonArray>();
		for (JsonVariant value : arr)
		{
			this->data.push_back(String(value.as<const char *>()));
		}
		//	}
		int command = doc["CommandId"];
		switch (command)
		{
		case SEFL::DirectCommands::PING_COMMAND:
		{
			this->getMqtt()->publish(this->getPubfeed().c_str(), data, 0, 2);
		}
		break;
		case SEFL::DirectCommands::STATUS_COMMAND:
		{

			char output[128];
			DynamicJsonDocument repDoc(SEFL::DOC_SIZE);
			repDoc["CommandId"] =
				static_cast<int>(SEFL::DirectCommands::STATUS_COMMAND);
			int tstatus = static_cast<int>(this->getStatus());
			repDoc["SubcommandId"] = tstatus;
			JsonArray jdata = repDoc.createNestedArray("Data");
			serializeJson(repDoc, output);
			this->getMqtt()->publish(this->getPubfeed().c_str(), output, 0, 2);
		}
		break;
		case SEFL::DirectCommands::ACTION_COMMAND:
		{
			this->setPowerStatus(true);
			enum SEFL::BasicClientStatuses status =
				static_cast<SEFL::BasicClientStatuses>(doc["SubcommandId"].as<int>());
			this->setStatus(status);

			this->getMqtt()->publish(this->getPubfeed().c_str(), data, 0, 2);
		}
		break;
		case SEFL::DirectCommands::DEACTIVATE_DEVICE_COMMAND:
		{
			this->setPowerStatus(false);
		}
		break;
		case SEFL::DirectCommands::DEBUG_MODE_COMMAND:
		{
			//	this->setPowerStatus(false);
		}
		break;
		case SEFL::DirectCommands::RESET_COMMAND:
		{
			this->setStatus((this->reset_status_));
			this->cleanData();
		}
		break;
		default:
		{
		}
		}
	}

	void Quest_Basic_Client::setStatus(uint8_t status)
	{
		if (this->status_ != static_cast<SEFL::BasicClientStatuses>(status))
		{
			status_ = static_cast<SEFL::BasicClientStatuses>(status);
			this->changed_status_ = true;
		}
	}

	Quest_Basic_Client::Quest_Basic_Client(MQTTClient &mqtt, const char *name,
										   uint8_t reset_status, const char *placement, const char *in_topic,
										   const char *out_topic, SEFL::Language language) : Quest_Client(mqtt, name, BASIC, reset_status, placement, in_topic,
																										  out_topic, language),
																							 status_(
																								 SEFL::BasicClientStatuses::DEFAULT_STATUS)
	{
	}

	void Quest_Basic_Client::reportStatus()
	{
		char output[128];
		DynamicJsonDocument repDoc(SEFL::DOC_SIZE);
		repDoc["CommandId"] =
			static_cast<int>(SEFL::DirectCommands::STATUS_TRIGGER_COMMAND);
		int tstatus = static_cast<int>(this->getStatus());
		repDoc["SubcommandId"] = tstatus;
		JsonArray jdata = repDoc.createNestedArray("Data");
		for (unsigned int i = 0; i < this->data.size(); i++)
		{
			if (this->data[i].length())
				jdata.add(this->data[i]);
		}
		serializeJson(repDoc, output);
		this->getMqtt()->publish(this->getPubfeed().c_str(), output, 0, 2);
	}

	SEFL::BasicClientStatuses Quest_Basic_Client::getStatus() const
	{
		return (status_);
	}

} /* namespace SEFL */

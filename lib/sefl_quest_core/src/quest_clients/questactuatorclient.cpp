/*
 * questactuatorclient.cpp
 *
 *  Created on: May 17, 2021
 *      Author: vladi
 */

#include <ArduinoJson.h>
#include "questactuatorclient.h"
#include <WString.h>
#include <MQTT.h>
namespace SEFL
{

	Quest_Actuator_Client::~Quest_Actuator_Client()
	{
		// TODO Auto-generated destructor stub
	}

	void Quest_Actuator_Client::act()
	{
		switch (this->getStatus())
		{
		case SEFL::ActuatorClientStatuses::OFF_STATUS:
		{
			this->onOff();
		}
		break;
		case SEFL::ActuatorClientStatuses::ON_STATUS:
		{
			this->onOn();
		}
		break;
		}
	}

	void Quest_Actuator_Client::inputClb(const char *data, uint16_t len)
	{

		//  { "CommandId": 0,
		//	  "SubcommandId":0,
		//    "Data": ["0"]
		//  }
		// Serial.println("Actuator got a mail:");
        StaticJsonDocument<SEFL::DOC_SIZE> doc;
		deserializeJson(doc, data, len);
		JsonArray arr = doc["Data"].as<JsonArray>();
		for (JsonVariant value : arr)
		{
			this->data.push_back(String(value.as<const char *>()));
		}

		int command = doc["CommandId"];
		switch (command)
		{
		case SEFL::DirectCommands::PING_COMMAND:
		{
			// this->getMqtt()->publish(this->getPubfeed().c_str(), data, 0, 2);
			publish(data);
		}
		break;
		case SEFL::DirectCommands::STATUS_TRIGGER_COMMAND:
		{

			char output[128];
            StaticJsonDocument<SEFL::DOC_SIZE> repDoc;
			repDoc["CommandId"] =
				static_cast<int>(SEFL::DirectCommands::STATUS_TRIGGER_COMMAND);
			repDoc["SubcommandId"] = 0;
			JsonArray jdata = repDoc.createNestedArray("Data");
			int tstatus = static_cast<int>(this->getStatus());
			jdata.add(tstatus);
			serializeJson(repDoc, output);
			// this->getMqtt()->publish(this->getPubfeed().c_str(), output, 0, 2);
			publish(output);
		}
		break;
		case SEFL::DirectCommands::STATUS_COMMAND:
		{

			char output[128];
            StaticJsonDocument<SEFL::DOC_SIZE> repDoc(SEFL::DOC_SIZE);
			repDoc["CommandId"] =
				static_cast<int>(SEFL::DirectCommands::STATUS_COMMAND);
			int tstatus = static_cast<int>(this->getStatus());
			repDoc["SubcommandId"] = tstatus;
			JsonArray jdata = repDoc.createNestedArray("Data");
			serializeJson(repDoc, output);
			// this->getMqtt()->publish(this->getPubfeed().c_str(), output, 0, 2);
			publish(output);
		}
		break;
		case SEFL::DirectCommands::ACTION_COMMAND:
		{
			this->setPowerStatus(true);
			enum SEFL::ActuatorClientStatuses status =
				static_cast<SEFL::ActuatorClientStatuses>(doc["SubcommandId"].as<int>());
			this->setStatus(status);
			// this->getMqtt()->publish(this->getPubfeed().c_str(), data, 0, 2);
			publish(data);
		}
		break;
		case SEFL::DirectCommands::DEACTIVATE_DEVICE_COMMAND:
		{
			this->setPowerStatus(false);
		}
		break;
		case SEFL::DirectCommands::DEBUG_MODE_COMMAND:
		{
		}
		break;
		case SEFL::DirectCommands::RESET_COMMAND:
		{
			this->setStatus(this->reset_status_);

            publish(data, 2);
			this->cleanData();
		}
		break;
		default:
		{
		}
		}
	}

	void Quest_Actuator_Client::setStatus(uint8_t status)
	{
		if (this->status_ != static_cast<SEFL::ActuatorClientStatuses>(status))
		{
			status_ = static_cast<SEFL::ActuatorClientStatuses>(status);
			this->changed_status_ = true;
		}
	}

	Quest_Actuator_Client::Quest_Actuator_Client(MQTTClient &mqtt, const char *name,
												 uint8_t reset_status, const char *placement, const char *in_topic,
												 const char *out_topic, SEFL::Language language) : Quest_Client(mqtt, name, ACTUATOR, reset_status, placement, in_topic,
																												out_topic, language),
																								   status_(
																									   SEFL::ActuatorClientStatuses::OFF_STATUS)
	{
	}

	SEFL::ActuatorClientStatuses Quest_Actuator_Client::getStatus()
	{
		return (status_);
	}

	void Quest_Actuator_Client::reportStatus()
	{
		char output[128];
        StaticJsonDocument<SEFL::DOC_SIZE> repDoc;
		repDoc["CommandId"] =
			static_cast<int>(SEFL::DirectCommands::STATUS_TRIGGER_COMMAND);
		int tstatus = static_cast<int>(this->getStatus());
		repDoc["SubcommandId"] = tstatus;
		JsonArray jdata = repDoc.createNestedArray("Data");
		for (auto & i : this->data)
		{
			if (i.length())
				jdata.add(i);
		}
		// TODO stuff
		serializeJson(repDoc, output);
		publish(output);
	}

} /* namespace SEFL */

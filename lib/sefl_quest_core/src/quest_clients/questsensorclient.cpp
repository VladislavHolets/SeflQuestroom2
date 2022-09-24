/*
 * questsensorclient.cpp
 *
 *  Created on: May 17, 2021
 *      Author: vladi
 */

#include <ArduinoJson.h>
#include <MQTT.h>
#include "questsensorclient.h"
#include <WString.h>

namespace SEFL
{
	class MQTT;
} /* namespace SEFL */

namespace SEFL
{

	Quest_Sensor_Client::~Quest_Sensor_Client()
	{
		// TODO Auto-generated destructor stub
	}

	void Quest_Sensor_Client::act()
	{
		switch (this->getStatus())
		{
		case SEFL::SensorClientStatuses::SET_STATUS:
		{
			this->onSet();
		}
		break;
		case SEFL::SensorClientStatuses::RESET_STATUS:
		{
			this->onReset();
			// this->cleanData();
		}
		break;
		}
	}

	void Quest_Sensor_Client::inputClb(const char *data, uint16_t len)
	{
		//  { "CommandId": 0,
		//    "Arguments": []
		//  }
		// Serial.println("Sensor client got a mail");
		DynamicJsonDocument doc(SEFL::DOC_SIZE);
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
			this->publish(data);

			//	this->getMqtt()->publish(this->getPubfeed().c_str(), data, 0, 2);
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
			// this->getMqtt()->publish(this->getPubfeed().c_str(), output, 0, 2);
			publish(output);
		}
		break;
		case SEFL::DirectCommands::ACTION_COMMAND:
		{
			this->setPowerStatus(true);
			enum SEFL::SensorClientStatuses status =
				static_cast<SEFL::SensorClientStatuses>(doc["SubcommandId"].as<int>());
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
			this->cleanData();
		}
		break;
		default:
		{
		}
		}
	}

	Quest_Sensor_Client::Quest_Sensor_Client(MQTTClient &mqtt, const char *name,
											 uint8_t reset_status, const char *placement, const char *in_topic,
											 const char *out_topic, SEFL::Language language) : Quest_Client(mqtt, name, SENSOR, reset_status, placement, in_topic,
																											out_topic, language),
																							   status_(
																								   SEFL::SensorClientStatuses::RESET_STATUS)
	{
	}

	SEFL::SensorClientStatuses Quest_Sensor_Client::getStatus()
	{
		return (this->status_);
	}

	void Quest_Sensor_Client::setStatus(uint8_t status)
	{
		if (this->status_ != static_cast<SEFL::SensorClientStatuses>(status))
		{
			status_ = static_cast<SEFL::SensorClientStatuses>(status);
			this->changed_status_ = true;
		}
	}

	void Quest_Sensor_Client::reportStatus()
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
		// TODO stuff
		serializeJson(repDoc, output);
		// Serial.println(output);
		this->getMqtt()->publish(this->getPubfeed().c_str(), output, 0, 2);
	}

} /* namespace SEFL */

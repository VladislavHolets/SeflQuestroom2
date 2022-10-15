/*
 * questhostclient.cpp
 *
 *  Created on: May 16, 2021
 *      Author: vladi
 */

#include <ArduinoJson.h>
#include "../utils/logger.h"
#include <AsyncMqtt_Generic.h>
#include "questhostclient.h"
#include <WString.h>

namespace SEFL
{

	Quest_Host_Client::~Quest_Host_Client()
	{
		// TODO Auto-generated destructor stub
	}

	void Quest_Host_Client::act()
	{
	}

	void Quest_Host_Client::inputClb(const char *data, uint16_t len)
	{
		//  { "CommandId": 0,
		//	  "SubcommandId":0,
		//    "Data": ["0"]
		//  }
		DynamicJsonDocument doc(SEFL::DOC_SIZE);
		deserializeJson(doc, data, len);
		if (!doc["Data"].getElement(0))
		{
			JsonArray arr = doc["Data"].as<JsonArray>();
			for (JsonVariant value : arr)
			{
				this->data.push_back(String(value.as<const char *>()));
			}
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
		case SEFL::DirectCommands::RESET_COMMAND:
		{
			this->reset_trigger_ = true;

			this->cleanData();
		}
		break;
		default:
		{
		}
		}
	}

	void Quest_Host_Client::reportStatus()
	{
	}

	Quest_Host_Client::Quest_Host_Client(MQTTClient &mqtt, const char *name,
										 const char *placement, const char *in_topic, const char *out_topic,
										 SEFL::Language language) : Quest_Client(mqtt, name, HOST, 0, placement, in_topic, out_topic,
																				 language),
																	reset_trigger_(false)
	{
		Logger::notice("host_device", F("constructor_done"));
	}

	void Quest_Host_Client::setStatus(uint8_t status)
	{
	}

} /* namespace SEFL */

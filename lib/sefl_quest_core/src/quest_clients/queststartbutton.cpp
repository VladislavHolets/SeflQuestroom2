/*
 * queststartbutton.cpp
 *
 *  Created on: May 17, 2021
 *      Author: vladi
 */

#include <ArduinoJson.h>
#include <MQTT.h>
#include "../peripherals/multpinsextender.h"
#include "queststartbutton.h"
#include <WString.h>

namespace SEFL
{
	class MQTT;
} /* namespace SEFL */

namespace SEFL
{

	Quest_Start_Button::Quest_Start_Button(MQTTClient &mqtt, uint8_t start_pin,
										   const char *name, uint8_t reset_status, const char *placement,
										   const char *in_topic, const char *out_topic, SEFL::Language language) : Quest_Client(mqtt, name, START_BUTTON, reset_status, placement,
																																in_topic, out_topic, language),
																												   status_(
																													   SEFL::StartButtonClientStatuses::DEFAULT_BUTTON_STATUS),
																												   start_pin_(
																													   start_pin)
	{
	}

	Quest_Start_Button::~Quest_Start_Button()
	{
		// TODO Auto-generated destructor stub
	}

	void Quest_Start_Button::act()
	{
        this->processDelayedPublications();
		switch (this->getStatus())
		{
		case SEFL::StartButtonClientStatuses::ACTIVATED_BUTTON_STATUS:
		{
			this->onActivated();
		}
		break;
		case SEFL::StartButtonClientStatuses::DEFAULT_BUTTON_STATUS:
		{
			this->onDefault();
		}
		break;
		case SEFL::StartButtonClientStatuses::DISABLED_BUTTON_STATUS:
		{
			this->onDisabled();
		}
		break;
		}
	}

	void Quest_Start_Button::inputClb(const char *data, uint16_t len)
	{
        StaticJsonDocument<SEFL::DOC_SIZE> doc;
		deserializeJson(doc, data, len);
		if (!doc["Data"][0])
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
		case SEFL::DirectCommands::STATUS_COMMAND:
		{

			char output[128];
            StaticJsonDocument<SEFL::DOC_SIZE> repDoc;
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
			//		char output[128];
			//		StaticJsonDocument<SEFL::DOC_SIZE> repDoc;
			//		repDoc["CommandId"] =
			//				static_cast<int>(SEFL::DirectCommands::ACTION_COMMAND);
			enum SEFL::StartButtonClientStatuses status =
				static_cast<SEFL::StartButtonClientStatuses>(doc["SubcommandId"].as<int>());
			this->setStatus(status);
			//
			//		JsonArray jdata = repDoc.createNestedArray("Data");
			//		serializeJson(repDoc, output);
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
			//	this->setPowerStatus(false);
		}
		break;
		case SEFL::DirectCommands::RESET_COMMAND:
		{
			this->setStatus(SEFL::StartButtonClientStatuses::DEFAULT_BUTTON_STATUS);
            publish(data);
			this->cleanData();
		}
		break;
		default:
		{
		}
		}
	}

	void Quest_Start_Button::setStatus(uint8_t status)
	{
		if (this->status_ != static_cast<SEFL::StartButtonClientStatuses>(status))
		{
			status_ = static_cast<SEFL::StartButtonClientStatuses>(status);
			this->changed_status_ = true;
		}
	}

	uint8_t Quest_Start_Button::getStartPin()
	{
		return (start_pin_);
	}

	void Quest_Start_Button::onDefault()
	{
		if (this->isChangedStatus())
		{
			this->unsetChangedStatus();
			this->reportStatus();
		}
		if (!Mext.digitalRead(start_pin_))
		{
			this->setStatus(ACTIVATED_BUTTON_STATUS);
		}
	}

	void Quest_Start_Button::onActivated()
	{
		if (this->isChangedStatus())
		{
			this->unsetChangedStatus();
			this->reportStatus();
		}
	}

	void Quest_Start_Button::onDisabled()
	{
		if (this->isChangedStatus())
		{
			this->unsetChangedStatus();
			this->reportStatus();
		}
	}

	SEFL::StartButtonClientStatuses Quest_Start_Button::getStatus()
	{
		return (status_);
	}

	void Quest_Start_Button::reportStatus()
	{
		char output[128];
        StaticJsonDocument<SEFL::DOC_SIZE> repDoc;
		repDoc["CommandId"] =
			static_cast<int>(SEFL::DirectCommands::STATUS_TRIGGER_COMMAND);
		int tstatus = static_cast<int>(this->getStatus());
		repDoc["SubcommandId"] = tstatus;
		JsonArray jdata = repDoc.createNestedArray("Data");
		for (int i = 0; i < this->data.size(); i++)
		{
			if (this->data[i].length())
				jdata.add(this->data[i]);
		}
		serializeJson(repDoc, output);
		// this->getMqtt()->publish(this->getPubfeed().c_str(), output, 0, 2);
		publish(output);
	}

} /* namespace SEFL */

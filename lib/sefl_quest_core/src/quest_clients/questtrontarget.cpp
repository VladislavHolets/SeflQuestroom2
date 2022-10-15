/*
 * questtrontarget.cpp
 *
 *  Created on: Jul 11, 2021
 *      Author: vladi
 */

#include <ArduinoJson.h>
#include <Array.h>
#include <IRremote.h>
#include <AsyncMqtt_Generic.h>
#include <sys/_stdint.h>
#include <WString.h>
#include "../constants.h"
#include "../utils/logger.h"
#include "../peripherals/multpinsextender.h"
#include "questtrontarget.h"
#define DECODE_NEC
namespace SEFL
{

	Quest_Tron_Target::~Quest_Tron_Target()
	{
		// TODO Auto-generated destructor stub
	}

	void Quest_Tron_Target::act()
	{
		switch (this->getStatus())
		{
		case SEFL::TronTargetClientStatuses::DISABLED_STATUS:
		{
			this->onDislabled();
		}
		break;
		case SEFL::TronTargetClientStatuses::DEAD_STATUS:
		{
			this->onDead();
		}
		break;
		case SEFL::TronTargetClientStatuses::ALIVE_STATUS:
		{
			this->onAlive();
		}
		break;
		}
	}

	void Quest_Tron_Target::setStatus(uint8_t status)
	{
		if (this->status_ != static_cast<SEFL::TronTargetClientStatuses>(status))
		{
			status_ = static_cast<SEFL::TronTargetClientStatuses>(status);
			this->changed_status_ = true;
		}
	}

	void Quest_Tron_Target::reportStatus()
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
		this->getMqtt()->publish(this->getPubfeed().c_str(), output, 1);
	}

	Quest_Tron_Target::Quest_Tron_Target(MQTTClient &mqtt, uint8_t receiver,
										 const char *name, uint8_t reset_status, const char *placement,
										 const char *in_topic, const char *out_topic, SEFL::Language language) : Quest_Client(mqtt, name, TRON_TARGET, reset_status, placement, in_topic,
																															  out_topic, language),
																												 status_(
																													 SEFL::TronTargetClientStatuses::DISABLED_STATUS),
																												 RECEIVER(
																													 receiver),
																												 health(0)
	{
		Mext.digitalRead(RECEIVER);
		IrReceiver.begin(Mext.getCi());
	}

	void Quest_Tron_Target::inputClb(const char *data, uint16_t len)
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
			this->getMqtt()->publish(this->getPubfeed().c_str(), data, 1);
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
			this->getMqtt()->publish(this->getPubfeed().c_str(), output, 1);
		}
		break;
		case SEFL::DirectCommands::ACTION_COMMAND:
		{
			this->setPowerStatus(true);
			enum SEFL::TronTargetClientStatuses status =
				static_cast<SEFL::TronTargetClientStatuses>(doc["SubcommandId"].as<int>());
			this->setStatus(status);

			this->getMqtt()->publish(this->getPubfeed().c_str(), data, 1);
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

	TronTargetClientStatuses Quest_Tron_Target::getStatus()
	{
		return (status_);
	}

	void Quest_Tron_Target::onDead()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			reportStatus();
		}
	}
	void Quest_Tron_Target::onDislabled()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->data.clear();
			reportStatus();
		}
	}

	void Quest_Tron_Target::onAlive()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			reportStatus();
		}
		while (!this->data.empty())
		{
			if (this->data.front().toInt())
			{
				health += this->data.front().toInt();
				this->data.remove(0);
			}
		}
		if (health == 0)
		{
			this->setStatus(SEFL::TronTargetClientStatuses::DEAD_STATUS);
		}
		SEFL::Logger::notice("target", "alive");
		if (IrReceiver.decode())
		{
			health = health - 1;
			SEFL::Logger::verbose(this->name_, health);
			String message = String("health left:") + String(health);
			this->data.push_back(message);
			this->data.pop_back();
			reportStatus();
			IrReceiver.resume();
		}
	}

} /* namespace SEFL */

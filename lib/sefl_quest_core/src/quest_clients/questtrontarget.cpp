/*
 * questtrontarget.cpp
 *
 *  Created on: Jul 11, 2021
 *      Author: vladi
 */

#include <ArduinoJson.h>
#include <Array.h>
#include <IRremote.h>
#include <MQTT.h>
#include <sys/_stdint.h>
#include <WString.h>
#include "../constants.h"
#include "../utils/logger.h"
#include "../peripherals/multpinsextender.h"
#include "questtrontarget.h"
#include "peripherals/pwmpinsextender.h"

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
		publish( output, 2);
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
        led_pins= nullptr;
        led_pins_size=-1;
        max_health=100;
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
			publish(data,2);
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
			publish(output, 2);
		}
		break;
		case SEFL::DirectCommands::ACTION_COMMAND:
		{
			this->setPowerStatus(true);
			enum SEFL::TronTargetClientStatuses status =
				static_cast<SEFL::TronTargetClientStatuses>(doc["SubcommandId"].as<int>());
			this->setStatus(status);

			publish(data, 2);
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
            health=0;
            displayHealth();
		}
	}
	void Quest_Tron_Target::onDislabled()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->data.clear();
			reportStatus();

            health=0;
            displayHealth();
		}
	}

	void Quest_Tron_Target::onAlive()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			reportStatus();

            displayHealth();
		}
		while (!this->data.empty())
		{
			if (this->data.front().toInt())
			{
				health += this->data.front().toInt();
				this->data.remove(0);
			}

            displayHealth();
		}
		if (health <= 0)
		{
            health=0;
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
            displayHealth();
		}
	}

    void Quest_Tron_Target::setLedPins(const uint8_t *ledPins,uint8_t ledPinsSize) {
        if(led_pins!= nullptr){
            delete [] led_pins;
        }
        led_pins_size=ledPinsSize;
        led_pins=new uint8_t[led_pins_size];
        for (int i = 0; i < led_pins_size; ++i) {
            led_pins[i]=ledPins[i];
        }
    }

    void Quest_Tron_Target::displayHealth() {
        if(led_pins_size<0){
            return;
        }
        for (int i = 0; i < led_pins_size; ++i) {
            int temp =this->health-(this->max_health / led_pins_size * i);
            temp=(temp>this->max_health / led_pins_size)?(this->max_health / led_pins_size):(temp);
            int value=((temp>0)?temp:0) * 4096 / (max_health / led_pins_size);
            Logger::notice(this->getName(),value);
            Pext.analogWrite(led_pins[i], (led_pins[i]>8)?value:(4096-value));
        }
    }

    void Quest_Tron_Target::setMaxHealth(int16_t maxHealth) {
        max_health = maxHealth;
    }

} /* namespace SEFL */

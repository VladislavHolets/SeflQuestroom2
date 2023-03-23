/*
 * questtimerclient.cpp
 *
 *  Created on: May 17, 2021
 *      Author: vladi
 */

#include <ArduinoJson.h>
#include "questtimerclient.h"
#include "../constants.h"
#include <MQTT.h>

namespace SEFL
{

	Quest_Timer_Client::~Quest_Timer_Client()
	{
		// TODO Auto-generated destructor stub
	}

	void Quest_Timer_Client::act()
	{
	}

	void Quest_Timer_Client::inputClb(const char *data, uint16_t len)
	{
	}

	void Quest_Timer_Client::setStatus(uint8_t status)
	{
	}

	void Quest_Timer_Client::onFull()
	{
	}

	void Quest_Timer_Client::onEnabled()
	{
	}

	void Quest_Timer_Client::onTriggered()
	{
	}

	void Quest_Timer_Client::onFree()
	{
	}

	Quest_Timer_Client::Quest_Timer_Client(MQTTClient &mqtt, const char *name,
										   uint8_t reset_status, const char *placement, const char *in_topic,
										   const char *out_topic, SEFL::Language language) : Quest_Client(mqtt, name, TIMER, reset_status, placement, in_topic,
																										  out_topic, language),
																							 status_(
																								 SEFL::TimerClientStatuses::FREE_STATUS)
	{
	}

	void Quest_Timer_Client::reportStatus()
	{
		char output[128];
        StaticJsonDocument<SEFL::DOC_SIZE> repDoc;
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
		this->getMqtt()->publish(this->getPubfeed().c_str(), output, 1);
	}

	SEFL::TimerClientStatuses Quest_Timer_Client::getStatus()
	{
		return (this->status_);
	}

} /* namespace SEFL */

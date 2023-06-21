/*
 * questclient.h
 *
 *  Created on: May 16, 2021
 *      Author: vladi
 */

#ifndef QUESTCLIENT_H_
#define QUESTCLIENT_H_

#include <Array.h>
#include "../constants.h"
#include "../mqtt_wrappers/mqtttwowayinteractor.h"
#include <sys/_stdint.h>
#include <WString.h>

namespace SEFL
{

	class Quest_Client : public MQTT_Two_Way_Interactor
	{
	protected:
		bool changed_status_;
		const char *name_;
		ClientType type_;
		bool power_status_;
		SEFL::Language language_;
		Array<String, MAX_DATA_LENGTH> data;
		void unsetChangedStatus();

	public:
		static const char *asString(ClientType client);
		const uint8_t reset_status_;
		void cleanData();
		void setChangedStatus(bool changedStatus);
		Quest_Client(MQTTClient &mqtt, const char *name, ClientType type = DEFAULT_DEVICE,
					 uint8_t reset_status = 1, const char *placement = DEFAULT_PLACEMENT,
					 const char *in_topic = DEFAUT_IN_TOPIC, const char *out_topic = DEFAUT_OUT_TOPIC, SEFL::Language language = UKR);
		~Quest_Client() override;
		virtual void act() = 0;
		virtual void reportStatus() = 0;
		bool isChangedStatus();
		Array<String, MAX_DATA_LENGTH> &getData();
		//const char *getInTopic();
		SEFL::Language getLanguage();
		void setLanguage(SEFL::Language language);
		const char *getName();
		//const char *getOutTopic();
		//const char *getPlacement();
		bool isPowerStatus();
		void setPowerStatus(bool powerStatus);
		ClientType getType();
		virtual void setStatus(uint8_t status) = 0;
	};

} /* namespace SEFL */

#endif /* QUESTCLIENT_H_ */

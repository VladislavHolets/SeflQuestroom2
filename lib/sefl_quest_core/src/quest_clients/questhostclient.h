/*
 * questhostclient.h
 *
 *  Created on: May 16, 2021
 *      Author: vladi
 */

#ifndef QUESTHOSTCLIENT_H_
#define QUESTHOSTCLIENT_H_

#include "../constants.h"
#include "questclient.h"
#include <sys/_stdint.h>

namespace SEFL
{
	class MQTT;
} /* namespace SEFL */

namespace SEFL
{

	class Quest_Host_Client : public Quest_Client
	{

	public:
		bool reset_trigger_;
		Quest_Host_Client(MQTTClient &mqtt, const char *name, const char *placement = DEFAULT_PLACEMENT, const char *in_topic = DEFAUT_IN_TOPIC,
						  const char *out_topic = DEFAUT_OUT_TOPIC, SEFL::Language language = UKR);
		virtual ~Quest_Host_Client();
		virtual void act() override;
		virtual void inputClb(const char *data, uint16_t len) override;
		virtual void reportStatus() override;
		virtual void setStatus(uint8_t status) override;
	};

} /* namespace SEFL */

#endif /* QUESTHOSTCLIENT_H_ */

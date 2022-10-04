/*
 * questsensorclient.h
 *
 *  Created on: May 17, 2021
 *      Author: vladi
 */

#ifndef QUESTSENSORCLIENT_H_
#define QUESTSENSORCLIENT_H_

#include "../constants.h"
#include "questclient.h"
#include <sys/_stdint.h>

namespace SEFL
{

	class Quest_Sensor_Client : public Quest_Client
	{
	protected:
		SEFL::SensorClientStatuses status_;

	public:
		Quest_Sensor_Client(MQTTClient &mqtt, const char *name = DEFAULT_DEVICE_NAME,
							uint8_t reset_status = 1, const char *placement = DEFAULT_PLACEMENT,
							const char *in_topic = DEFAUT_IN_TOPIC, const char *out_topic = DEFAUT_OUT_TOPIC, SEFL::Language language = UKR);
		virtual ~Quest_Sensor_Client();
		virtual void act() override;
		virtual void inputClb(const char *data, uint16_t len) override;
		virtual void setStatus(uint8_t status) override;
		virtual void reportStatus() override;

		virtual void onSet() = 0;
		virtual void onReset() = 0;

		SEFL::SensorClientStatuses getStatus();
	};

} /* namespace SEFL */

#endif /* QUESTSENSORCLIENT_H_ */

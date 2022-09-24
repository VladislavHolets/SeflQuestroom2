/*
 * questtrontarget.h
 *
 *  Created on: Jul 11, 2021
 *      Author: vladi
 */

#ifndef QUESTTRONTARGET_H_
#define QUESTTRONTARGET_H_

#include "questclient.h"
namespace SEFL
{
	enum TronTargetClientStatuses
	{
		DISABLED_STATUS = 1,
		ALIVE_STATUS = 2,
		DEAD_STATUS = 3
	};
	class Quest_Tron_Target : public Quest_Client
	{
		TronTargetClientStatuses status_;
		const uint8_t RECEIVER = 0;
		int16_t health;

	public:
		Quest_Tron_Target(MQTTClient &mqtt, uint8_t receiver, const char *name = DEFAULT_DEVICE_NAME,
						  uint8_t reset_status = 1, const char *placement = DEFAULT_PLACEMENT,
						  const char *in_topic = DEFAUT_IN_TOPIC, const char *out_topic = DEFAUT_OUT_TOPIC, SEFL::Language language = UKR);
		virtual ~Quest_Tron_Target();
		virtual void act() override;
		virtual void setStatus(uint8_t status) override;
		virtual void reportStatus() override;
		virtual void onDislabled();
		virtual void onAlive();
		virtual void onDead();
		virtual void inputClb(const char *data, uint16_t len) override;
		TronTargetClientStatuses getStatus();
	};

} /* namespace SEFL */

#endif /* QUESTTRONTARGET_H_ */

/*
 * questtimerclient.h
 *
 *  Created on: May 17, 2021
 *      Author: vladi
 */

#ifndef QUESTTIMERCLIENT_H_
#define QUESTTIMERCLIENT_H_

#include <Array.h>
#include <quest_clients/questclient.h>
#include <sys/_stdint.h>
#include <WString.h>

namespace SEFL
{

	class Quest_Timer_Client : public Quest_Client
	{
		struct Alarm
		{
			uint32_t timestamp;
			String description;
		};
		Array<Alarm, 32> alarms_;
		SEFL::TimerClientStatuses status_;

	public:
		Quest_Timer_Client(MQTTClient &mqtt, const char *name = DEFAULT_DEVICE_NAME,
						   uint8_t reset_status = 1, const char *placement = DEFAULT_PLACEMENT,
						   const char *in_topic = DEFAUT_IN_TOPIC, const char *out_topic = DEFAUT_OUT_TOPIC, SEFL::Language language = UKR);
		virtual ~Quest_Timer_Client();
		virtual void act() override;
		void onFull();
		void onEnabled();
		void onTriggered();
		void onFree();
		virtual void inputClb(const char *data, uint16_t len) override;
		virtual void setStatus(uint8_t status) override;
		virtual void reportStatus() override;
		SEFL::TimerClientStatuses getStatus() const;
	};

} /* namespace SEFL */

#endif /* QUESTTIMERCLIENT_H_ */

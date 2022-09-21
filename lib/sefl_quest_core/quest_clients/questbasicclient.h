/*
 * questbasicclient.h
 *
 *  Created on: May 16, 2021
 *      Author: vladi
 */

#ifndef QUESTBASICCLIENT_H_
#define QUESTBASICCLIENT_H_

#include <constants.h>
#include <quest_clients/questclient.h>
#include <sys/_stdint.h>

namespace SEFL
{

	class Quest_Basic_Client : public Quest_Client
	{
	protected:
		SEFL::BasicClientStatuses status_;

	public:
		Quest_Basic_Client(MQTTClient &mqtt,
						   const char *name = DEFAULT_DEVICE_NAME, uint8_t reset_status = 1, const char *placement = DEFAULT_PLACEMENT, const char *in_topic = DEFAUT_IN_TOPIC,
						   const char *out_topic = DEFAUT_OUT_TOPIC, SEFL::Language language = UKR);
		virtual ~Quest_Basic_Client();
		virtual void act() override;
		virtual void inputClb(const char *data, uint16_t len) override;
		virtual void setStatus(uint8_t status) override;
		virtual void reportStatus() override;
		SEFL::BasicClientStatuses getStatus() const;
		virtual void onActive() = 0;
		virtual void onDefault() = 0;
		virtual void onFinished() = 0;
		virtual void onManualFinished() = 0;
	};

} /* namespace SEFL */

#endif /* QUESTBASICCLIENT_H_ */

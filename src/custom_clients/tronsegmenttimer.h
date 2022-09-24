/*
 * tronsegmenttimer.h
 *
 *  Created on: Aug 16, 2021
 *      Author: vladi
 */

#ifndef TRONSEGMENTTIMER_H_
#define TRONSEGMENTTIMER_H_

#include <peripherals/hc595driver.h>
#include <constants.h>
#include <quest_clients/questbasicclient.h>
#include <sys/_stdint.h>

namespace SEFL
{

	class Tron_Segment_Timer : public Quest_Basic_Client
	{
		HC595_cfg cfg_;

	public:
		Tron_Segment_Timer(MQTTClient &mqtt, HC595_cfg cfg, const char *name = DEFAULT_DEVICE_NAME,
						   uint8_t reset_status = 1, const char *placement = DEFAULT_PLACEMENT,
						   const char *in_topic = DEFAUT_IN_TOPIC, const char *out_topic = DEFAUT_OUT_TOPIC, SEFL::Language language = UKR);
		virtual ~Tron_Segment_Timer();
		virtual void onActive() override;
		virtual void onFinished() override;
		virtual void onManualFinished() override;
		virtual void onDefault() override;
	};

} /* namespace SEFL */

#endif /* TRONSEGMENTTIMER_H_ */

/*
 * home.h
 *
 *  Created on: Sep 18, 2022
 *      Author: piatk
 */

#ifndef HOME_H_
#define HOME_H_

#include "sefl_quest_core.h"

namespace SEFL
{

	class Home : public Quest_Basic_Client
	{
		uint16_t count;

	public:
		Home(MQTTClient &mqtt, const char *name = DEFAULT_DEVICE_NAME,
			 uint8_t reset_status = 1, const char *placement = DEFAULT_PLACEMENT,
			 const char *in_topic = DEFAUT_IN_TOPIC, const char *out_topic = DEFAUT_OUT_TOPIC, SEFL::Language language = UKR);
		virtual ~Home();
		virtual void onActive() override;
		virtual void onFinished() override;
		virtual void onManualFinished() override;
		virtual void onDefault() override;
		uint16_t getCount();
		void setCount(uint16_t count);
	};

} /* namespace SEFL */

#endif /* HOME_H_ */

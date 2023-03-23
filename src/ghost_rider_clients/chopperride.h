/*
 * chopperride.h
 *
 *  Created on: Jan 31, 2022
 *      Author: piatk
 */

#ifndef CHOPPERRIDE_H_
#define CHOPPERRIDE_H_
#include "sefl_quest_core.h"

#include <sys/_stdint.h>

class DFRobotDFPlayerMini;

namespace SEFL
{

	class Chopperride : public Quest_Basic_Client
	{

		uint8_t chopperStarter, chopperLeftButton, chopperRightButton, chopperThrottle, flagThrottle;
		uint8_t chopperLight;
		uint8_t chopperAddressLed;
		uint8_t ContactMagnet;
		uint64_t timer, timerThrottle;
		DFRobotDFPlayerMini *player;

	public:
		Chopperride(MQTTClient &mqtt, const char *name = DEFAULT_DEVICE_NAME,
					uint8_t reset_status = 1, const char *placement = DEFAULT_PLACEMENT,
					const char *in_topic = DEFAUT_IN_TOPIC, const char *out_topic = DEFAUT_OUT_TOPIC, SEFL::Language language = UKR);
		virtual ~Chopperride();
		virtual void onActive() override;
		virtual void onFinished() override;
		virtual void onManualFinished() override;
		virtual void onDefault() override;
		uint8_t getChopperAddressLed();
		void setChopperAddressLed(uint8_t chopperAddressLed);
		uint8_t getChopperLeftButton();
		void setChopperLeftButton(uint8_t chopperLeftButton);
		uint8_t getChopperLight();
		void setChopperLight(uint8_t chopperLight);
		uint8_t getChopperRightButton();
		void setChopperRightButton(uint8_t chopperRightButton);
		uint8_t getChopperStarter();
		void setChopperStarter(uint8_t chopperStarter);
		uint8_t getChopperThrottle();
		void setChopperThrottle(uint8_t chopperThrottle);
		uint8_t getContactMagnet();
		void setContactMagnet(uint8_t contactMagnet);
		DFRobotDFPlayerMini *getPlayer();
		void setPlayer(DFRobotDFPlayerMini *player);
		uint64_t getTimer();
		void setTimer(uint64_t timer);
		uint8_t getFlagThrottle();
		void setFlagThrottle(uint8_t flagThrottle);
		uint64_t getTimerThrottle();
		void setTimerThrottle(uint64_t timerThrottle);
	};

} /* namespace SEFL */

#endif /* CHOPPERRIDE_H_ */

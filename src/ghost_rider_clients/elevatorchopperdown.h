/*
 * elevatorchopperdown.h
 *
 *  Created on: Jan 31, 2022
 *      Author: piatk
 */

#ifndef ELEVATORCHOPPERDOWN_H_
#define ELEVATORCHOPPERDOWN_H_
#include "sefl_quest_core.h"

#include <sys/_stdint.h>

namespace SEFL
{

	class ElevatorChopperDown : public Quest_Basic_Client
	{

		uint8_t cargoLiftSwitch, winchBottomSwitch;
		uint8_t fabricMagnet, chopperLight;
		uint8_t winchOnRelay, winchDownRelay;
		uint64_t timer;

		bool switchFlag = false;

	public:
		ElevatorChopperDown(MQTTClient &mqtt, const char *name = DEFAULT_DEVICE_NAME,
							uint8_t reset_status = 1, const char *placement = DEFAULT_PLACEMENT,
							const char *in_topic = DEFAUT_IN_TOPIC, const char *out_topic = DEFAUT_OUT_TOPIC, SEFL::Language language = UKR);
		virtual ~ElevatorChopperDown();
		virtual void onActive() override;
		virtual void onFinished() override;
		virtual void onManualFinished() override;
		virtual void onDefault() override;
		uint8_t getCargoLiftSwitch();
		void setCargoLiftSwitch(uint8_t cargoLiftSwitch);

		uint8_t getFabricMagnet();
		void setFabricMagnet(uint8_t fabricMagnet);
		uint8_t getWinchBottomSwitch();
		void setWinchBottomSwitch(uint8_t winchBottomSwitch);
		uint8_t getWinchDownRelay();
		void setWinchDownRelay(uint8_t winchDownRelay);
		uint8_t getWinchOnRelay();
		void setWinchOnRelay(uint8_t winchOnRelay);
		uint8_t getChopperLight();
		void setChopperLight(uint8_t chopperLight);
		uint64_t getTimer();
		void setTimer(uint64_t timer);
	};

} /* namespace SEFL */

#endif /* ELEVATORCHOPPERDOWN_H_ */

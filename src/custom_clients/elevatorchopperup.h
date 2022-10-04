/*
 * elevatorchopperup.h
 *
 *  Created on: Jan 31, 2022
 *      Author: piatk
 */

#ifndef ELEVATORCHOPPERUP_H_
#define ELEVATORCHOPPERUP_H_

#include <sefl_quest_core.h>

#include <sys/_stdint.h>

namespace SEFL
{

	class ElevatorChopperUp : public Quest_Basic_Client
	{
		uint8_t cargoLiftUSwitch, winchTopSwitch;
		uint8_t winchOnRelay, winchUpRelay;

	public:
		ElevatorChopperUp(MQTTClient &mqtt, const char *name = DEFAULT_DEVICE_NAME,
						  uint8_t reset_status = 1, const char *placement = DEFAULT_PLACEMENT,
						  const char *in_topic = DEFAUT_IN_TOPIC, const char *out_topic = DEFAUT_OUT_TOPIC, SEFL::Language language = UKR);
		virtual ~ElevatorChopperUp();
		virtual void onActive() override;
		virtual void onFinished() override;
		virtual void onManualFinished() override;
		virtual void onDefault() override;
		uint8_t getCargoLiftSwitch();
		void setCargoLiftSwitch(uint8_t cargoLiftSwitch);
		uint8_t getWinchOnRelay();
		void setWinchOnRelay(uint8_t winchOnRelay);
		uint8_t getWinchTopSwitch();
		void setWinchTopSwitch(uint8_t winchTopSwitch);
		uint8_t getWinchUpRelay();
		void setWinchUpRelay(uint8_t winchUpRelay);
		uint8_t getCargoLiftUSwitch();
		void setCargoLiftUSwitch(uint8_t cargoLiftUSwitch);
	};

} /* namespace SEFL */

#endif /* ELEVATORCHOPPERUP_H_ */

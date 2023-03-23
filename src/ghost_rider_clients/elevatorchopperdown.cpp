/*
 * elevatorchopperdown.cpp
 *
 *  Created on: Jan 31, 2022
 *      Author: piatk
 */

#include "elevatorchopperdown.h"
#include <wiring_constants.h>
#include <wiring_digital.h>

namespace SEFL
{

	ElevatorChopperDown::ElevatorChopperDown(MQTTClient &mqtt, const char *name,
											 uint8_t reset_status, const char *placement, const char *in_topic,
											 const char *out_topic, SEFL::Language language) : Quest_Basic_Client(mqtt, name, reset_status, placement, in_topic,
																												  out_topic, language)
	{

		cargoLiftSwitch = 0;
		winchBottomSwitch = 1;

		winchOnRelay = 0;
		winchDownRelay = 1;
		fabricMagnet = 2;
		chopperLight = 3;
		timer = 0;
	}

	void ElevatorChopperDown::onActive()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("ElevatorChopperDown", "onActive");
			switchFlag = false;
			Pext.digitalWrite(fabricMagnet, true);
			Pext.digitalWrite(chopperLight, false);
			timer = millis();
		}

		Mext.digitalRead(winchBottomSwitch);
		pinMode(Mext.getCi(), INPUT_PULLUP);

		if (!switchFlag && Mext.digitalRead(winchBottomSwitch) && millis() - timer > 3500)
		{

			Pext.digitalWrite(winchOnRelay, false);
			Pext.digitalWrite(winchDownRelay, false);
		}

		if (!Mext.digitalRead(winchBottomSwitch))
		{
			Pext.digitalWrite(winchOnRelay, true);
			Pext.digitalWrite(winchDownRelay, true);
			switchFlag = true;

			this->setStatus(BasicClientStatuses::FINISHED_STATUS);
		}
	}

	void ElevatorChopperDown::onFinished()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			Pext.digitalWrite(winchOnRelay, true);
			Pext.digitalWrite(winchDownRelay, true);
			Pext.digitalWrite(chopperLight, true);
			SEFL::Logger::verbose("ElevatorChopperDown", "onFinished");
		}
	}

	void ElevatorChopperDown::onManualFinished()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			Pext.digitalWrite(winchOnRelay, true);
			Pext.digitalWrite(winchDownRelay, true);
			Pext.digitalWrite(chopperLight, true);
			SEFL::Logger::verbose("ElevatorChopperDown", "onManualFinished");
		}
	}

	void ElevatorChopperDown::onDefault()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			Pext.digitalWrite(winchOnRelay, true);
			Pext.digitalWrite(winchDownRelay, true);
			Pext.digitalWrite(fabricMagnet, false);
			SEFL::Logger::verbose("ElevatorChopperDown", "onDefault");
		}
	}

	uint8_t ElevatorChopperDown::getCargoLiftSwitch()
	{
		return cargoLiftSwitch;
	}

	void ElevatorChopperDown::setCargoLiftSwitch(uint8_t cargoLiftSwitch)
	{
		this->cargoLiftSwitch = cargoLiftSwitch;
	}

	uint8_t ElevatorChopperDown::getFabricMagnet()
	{
		return fabricMagnet;
	}

	void ElevatorChopperDown::setFabricMagnet(uint8_t fabricMagnet)
	{
		this->fabricMagnet = fabricMagnet;
	}

	uint8_t ElevatorChopperDown::getWinchBottomSwitch()
	{
		return winchBottomSwitch;
	}

	void ElevatorChopperDown::setWinchBottomSwitch(uint8_t winchBottomSwitch)
	{
		this->winchBottomSwitch = winchBottomSwitch;
	}

	uint8_t ElevatorChopperDown::getWinchDownRelay()
	{
		return winchDownRelay;
	}

	void ElevatorChopperDown::setWinchDownRelay(uint8_t winchDownRelay)
	{
		this->winchDownRelay = winchDownRelay;
	}

	uint8_t ElevatorChopperDown::getWinchOnRelay()
	{
		return winchOnRelay;
	}

	uint8_t ElevatorChopperDown::getChopperLight()
	{
		return chopperLight;
	}

	uint64_t ElevatorChopperDown::getTimer()
	{
		return timer;
	}

	void ElevatorChopperDown::setTimer(uint64_t timer)
	{
		this->timer = timer;
	}

	void ElevatorChopperDown::setChopperLight(uint8_t chopperLight)
	{
		this->chopperLight = chopperLight;
	}

	void ElevatorChopperDown::setWinchOnRelay(uint8_t winchOnRelay)
	{
		this->winchOnRelay = winchOnRelay;
	}

	ElevatorChopperDown::~ElevatorChopperDown()
	{
		// TODO Auto-generated destructor stub
	}

} /* namespace SEFL */

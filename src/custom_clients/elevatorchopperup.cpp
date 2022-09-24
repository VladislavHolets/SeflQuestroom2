/*
 * elevatorchopperup.cpp
 *
 *  Created on: Jan 31, 2022
 *      Author: piatk
 */

#include <custom_clients/elevatorchopperup.h>
#include <utils/logger.h>
#include <peripherals/multpinsextender.h>
#include <peripherals/pwmpinsextender.h>
#include <wiring_constants.h>
#include <wiring_digital.h>

bool upswitchFlag;
namespace SEFL
{

	ElevatorChopperUp::ElevatorChopperUp(MQTTClient &mqtt, const char *name,
										 uint8_t reset_status, const char *placement, const char *in_topic,
										 const char *out_topic, SEFL::Language language) : Quest_Basic_Client(mqtt, name, reset_status, placement, in_topic,
																											  out_topic, language)
	{

		cargoLiftUSwitch = 0;
		winchTopSwitch = 2;

		winchOnRelay = 0;
		winchUpRelay = 2;
	}

	void ElevatorChopperUp::onActive()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("ElevatorChopperUp", "onActive");
			upswitchFlag = false;
		}

		Mext.digitalRead(winchTopSwitch);
		pinMode(Mext.getCi(), INPUT_PULLUP);

		if (!upswitchFlag && Mext.digitalRead(winchTopSwitch))
		{

			Pext.digitalWrite(winchOnRelay, false);
			Pext.digitalWrite(winchUpRelay, false);
		}

		if (!Mext.digitalRead(winchTopSwitch))
		{
			Pext.digitalWrite(winchOnRelay, true);
			Pext.digitalWrite(winchUpRelay, true);
			upswitchFlag = true;

			this->setStatus(BasicClientStatuses::FINISHED_STATUS);
		}
	}

	void ElevatorChopperUp::onFinished()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			Pext.digitalWrite(winchOnRelay, true);
			Pext.digitalWrite(winchUpRelay, true);
			SEFL::Logger::verbose("ElevatorChopperUp", "onFinished");
		}
	}

	void ElevatorChopperUp::onManualFinished()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			Pext.digitalWrite(winchOnRelay, true);
			Pext.digitalWrite(winchUpRelay, true);
			SEFL::Logger::verbose("ElevatorChopperUp", "onManualFinished");
		}
	}

	void ElevatorChopperUp::onDefault()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			Pext.digitalWrite(winchOnRelay, true);
			Pext.digitalWrite(winchUpRelay, true);
			SEFL::Logger::verbose("ElevatorChopperUp", "onDefault");
		}
	}

	uint8_t ElevatorChopperUp::getWinchOnRelay()
	{
		return winchOnRelay;
	}

	void ElevatorChopperUp::setWinchOnRelay(uint8_t winchOnRelay)
	{
		this->winchOnRelay = winchOnRelay;
	}

	uint8_t ElevatorChopperUp::getWinchTopSwitch()
	{
		return winchTopSwitch;
	}

	void ElevatorChopperUp::setWinchTopSwitch(uint8_t winchTopSwitch)
	{
		this->winchTopSwitch = winchTopSwitch;
	}

	uint8_t ElevatorChopperUp::getWinchUpRelay()
	{
		return winchUpRelay;
	}

	uint8_t ElevatorChopperUp::getCargoLiftUSwitch()
	{
		return cargoLiftUSwitch;
	}

	void ElevatorChopperUp::setCargoLiftUSwitch(uint8_t cargoLiftUSwitch)
	{
		this->cargoLiftUSwitch = cargoLiftUSwitch;
	}

	void ElevatorChopperUp::setWinchUpRelay(uint8_t winchUpRelay)
	{
		this->winchUpRelay = winchUpRelay;
	}

	ElevatorChopperUp::~ElevatorChopperUp()
	{
		// TODO Auto-generated destructor stub
	}

} /* namespace SEFL */

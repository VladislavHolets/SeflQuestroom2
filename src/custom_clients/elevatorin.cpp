/*
 * elevatorin.cpp
 *
 *  Created on: Jan 31, 2022
 *      Author: piatk
 */

#include <DFRobotDFPlayerMini.h>
#include "elevatorin.h"
#include "magnet.h"
#include <wiring_constants.h>
#include <wiring_digital.h>

namespace SEFL
{

	Elevatorin::Elevatorin(MQTTClient &mqtt, const char *name, uint8_t reset_status,
						   const char *placement, const char *in_topic, const char *out_topic,
						   SEFL::Language language) : Quest_Basic_Client(mqtt, name, reset_status, placement, in_topic,
																		 out_topic, language)
	{
		elevatorInSwith = 0;
		elevatorOutSwith = 1;
		elevatorInButton = 2;
		elevatorInLed = 0;
		player = nullptr;
		timer = 0;
	}

	void Elevatorin::onActive()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("elevatorin", "onActive");

			if (player != nullptr)
			{
				player->volume(25);
			}
			Mext.digitalRead(elevatorInButton);
			pinMode(Mext.getCi(), INPUT_PULLUP);
			timer = millis();
		}

		//--------------------------------------------------------------------------
		//		if (this->data.front().equals("1")) {
		//			this->data.remove(0);
		//			out_magnet->setStatus(SEFL::ActuatorClientStatuses::ON_STATUS);
		//			in_magnet->setStatus(SEFL::ActuatorClientStatuses::OFF_STATUS);
		//
		//		}
		//		if (this->data.front().equals("2")) {
		//			this->data.remove(0);
		//			out_magnet->setStatus(SEFL::ActuatorClientStatuses::OFF_STATUS);
		//			in_magnet->setStatus(SEFL::ActuatorClientStatuses::ON_STATUS);
		//
		//		}
		//		if (this->data.front().equals("3")) {
		//			this->data.remove(0);
		//			out_magnet->setStatus(SEFL::ActuatorClientStatuses::OFF_STATUS);
		//			in_magnet->setStatus(SEFL::ActuatorClientStatuses::OFF_STATUS);
		//
		//		}
		//--------------------------------------------------------------------------

		if (!Mext.digitalRead(elevatorInSwith) && !Mext.digitalRead(elevatorOutSwith) && millis() - timer > 6000)
		{
			Pext.digitalWrite(elevatorInLed, HIGH);
		}
		else
		{
			Pext.digitalWrite(elevatorInLed, LOW);
		}

		//	SEFL::Logger::verbose("elevatorInButton",
		//			Mext.digitalRead(elevatorInButton));
		//	SEFL::Logger::verbose("elevatorInSwith		",
		//			Mext.digitalRead(elevatorInSwith));
		//	SEFL::Logger::verbose("elevatorOutSwith				",
		//			Mext.digitalRead(elevatorOutSwith));

		if (!Mext.digitalRead(elevatorInButton) && !Mext.digitalRead(elevatorInSwith) && !Mext.digitalRead(elevatorOutSwith) && millis() - timer > 6000)
		{
			if (player != nullptr)
			{
				player->play(4);
				SEFL::Logger::verbose("Elevatorin", "0004.mp3");
			}
			SEFL::Logger::verbose("elevatorOutSwith", "step1");
			//	out_magnet->setStatus(SEFL::ActuatorClientStatuses::ON_STATUS);
			SEFL::Logger::verbose("elevatorOutSwith", "step2");
			//	in_magnet->setStatus(SEFL::ActuatorClientStatuses::OFF_STATUS);
			SEFL::Logger::verbose("elevatorOutSwith", "step3");
			this->setStatus(BasicClientStatuses::FINISHED_STATUS);
		}
	}

	void Elevatorin::onFinished()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			Pext.digitalWrite(elevatorInLed, LOW);
			SEFL::Logger::verbose("Elevatorin", "onFinished");

			out_magnet->setStatus(SEFL::ActuatorClientStatuses::ON_STATUS);
			in_magnet->setStatus(SEFL::ActuatorClientStatuses::OFF_STATUS);
		}
	}

	void Elevatorin::onManualFinished()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();

			SEFL::Logger::verbose("Elevatorin", "onManualFinished");
			Pext.digitalWrite(elevatorInLed, LOW);

			out_magnet->setStatus(SEFL::ActuatorClientStatuses::ON_STATUS);
			in_magnet->setStatus(SEFL::ActuatorClientStatuses::OFF_STATUS);
		}
	}

	void Elevatorin::onDefault()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();

			SEFL::Logger::verbose("Elevatorin", "onDefault");
			Pext.digitalWrite(elevatorInLed, LOW);
			if (player != nullptr)
			{
				player->stop();
			}
		}
	}

	void Elevatorin::setPlayer(DFRobotDFPlayerMini *player)
	{
		this->player = player;
	}

	uint8_t Elevatorin::getElevatorInButton()
	{
		return elevatorInButton;
	}

	void Elevatorin::setElevatorInButton(uint8_t elevatorInButton)
	{
		this->elevatorInButton = elevatorInButton;
	}

	uint8_t Elevatorin::getElevatorInLed()
	{
		return elevatorInLed;
	}

	void Elevatorin::setElevatorInLed(uint8_t elevatorInLed)
	{
		this->elevatorInLed = elevatorInLed;
	}

	uint8_t Elevatorin::getElevatorInSwith()
	{
		return elevatorInSwith;
	}

	void Elevatorin::setElevatorInSwith(uint8_t elevatorInSwith)
	{
		this->elevatorInSwith = elevatorInSwith;
	}

	uint8_t Elevatorin::getElevatorOutSwith()
	{
		return elevatorOutSwith;
	}

	Magnet *Elevatorin::getOutMagnet()
	{
		return out_magnet;
	}

	uint64_t Elevatorin::getTimer()
	{
		return timer;
	}

	void Elevatorin::setTimer(uint64_t timer)
	{
		this->timer = timer;
	}

	void Elevatorin::setOutMagnet(Magnet *outMagnet)
	{
		out_magnet = outMagnet;
	}

	Magnet *Elevatorin::getInMagnet()
	{
		return in_magnet;
	}

	void Elevatorin::setInMagnet(Magnet *inMagnet)
	{
		in_magnet = inMagnet;
	}

	void Elevatorin::setElevatorOutSwith(uint8_t elevatorOutSwith)
	{
		this->elevatorOutSwith = elevatorOutSwith;
	}

	Elevatorin::~Elevatorin()
	{
		// TODO Auto-generated destructor stub
	}

} /* namespace SEFL */

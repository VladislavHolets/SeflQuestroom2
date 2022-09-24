/*
 * elevatorout.cpp
 *
 *  Created on: Jan 31, 2022
 *      Author: piatk
 */

#include <DFRobotDFPlayerMini.h>
#include <custom_clients/elevatorout.h>
#include <custom_clients/magnet.h>
#include <utils/logger.h>
#include <peripherals/multpinsextender.h>
#include <wiring_constants.h>
#include <wiring_digital.h>
#include <wiring_time.h>

unsigned long timing;

namespace SEFL
{

	Elevatorout::Elevatorout(MQTTClient &mqtt, const char *name, uint8_t reset_status,
							 const char *placement, const char *in_topic, const char *out_topic,
							 SEFL::Language language) : Quest_Basic_Client(mqtt, name, reset_status, placement, in_topic,
																		   out_topic, language)
	{
		elevatorInSwith = 0;
		elevatorOutSwith = 1;
		flag = 0;
		player = nullptr;
	}

	Elevatorout::~Elevatorout()
	{
		// TODO Auto-generated destructor stub
	}

	void Elevatorout::onActive()
	{

		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			timing = millis();
			if (player != nullptr)
			{
				player->volume(25);
				player->play(4);
				SEFL::Logger::verbose("Elevatorin", "0004.mp3");
			}
			Mext.digitalRead(elevatorOutSwith);
			pinMode(Mext.getCi(), INPUT_PULLUP);
			flag = 0;
		}
		//--------------------------------------------------------------------------
		if (this->data.front().equals("1"))
		{
			this->data.remove(0);
			out_magnet->setStatus(SEFL::ActuatorClientStatuses::ON_STATUS);
			in_magnet->setStatus(SEFL::ActuatorClientStatuses::OFF_STATUS);
		}
		if (this->data.front().equals("2"))
		{
			this->data.remove(0);
			out_magnet->setStatus(SEFL::ActuatorClientStatuses::OFF_STATUS);
			in_magnet->setStatus(SEFL::ActuatorClientStatuses::ON_STATUS);
		}
		if (this->data.front().equals("3"))
		{
			this->data.remove(0);
			out_magnet->setStatus(SEFL::ActuatorClientStatuses::OFF_STATUS);
			in_magnet->setStatus(SEFL::ActuatorClientStatuses::OFF_STATUS);
		}
		//--------------------------------------------------------------------------

		if (flag == 0)
		{
			if (Mext.digitalRead(elevatorOutSwith) == 0 && Mext.digitalRead(elevatorInSwith) == 0 && millis() - timing > 200)
			{
				flag = 1;
				out_magnet->setStatus(SEFL::ActuatorClientStatuses::OFF_STATUS);
				// in_magnet->setStatus(SEFL::ActuatorClientStatuses::ON_STATUS);

				SEFL::Logger::verbose("Elevatorout", "Door is open");
			}
			if (Mext.digitalRead(elevatorOutSwith) == 1 && Mext.digitalRead(elevatorInSwith) == 1)
			{
				timing = millis();
			}
		}

		if (flag == 1)
		{
			if (Mext.digitalRead(elevatorOutSwith) == 1 && millis() - timing > 300)
			{
				// SEFL::Logger::verbose("Elevatorout", "Door is close");
				this->setStatus(BasicClientStatuses::FINISHED_STATUS);
			}

			if (Mext.digitalRead(elevatorOutSwith) == 0)
			{
				timing = millis();
			}
		}

		SEFL::Logger::verbose("Elevatorout", millis() - timing);
		SEFL::Logger::verbose("flag		", flag);
		SEFL::Logger::verbose("Swith				", Mext.digitalRead(elevatorOutSwith));

		//	SEFL::Logger::verbose("Elevatorout", millis() -timing);
		//  if (millis() - timing > 3000 ){
		//	  this->setStatus(BasicClientStatuses::FINISHED_STATUS);
		//  }
	}

	void Elevatorout::onFinished()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();

			out_magnet->setStatus(SEFL::ActuatorClientStatuses::OFF_STATUS);
			in_magnet->setStatus(SEFL::ActuatorClientStatuses::ON_STATUS);

			SEFL::Logger::verbose("Elevatorout", "onFinished");
		}
	}

	void Elevatorout::onManualFinished()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();

			out_magnet->setStatus(SEFL::ActuatorClientStatuses::OFF_STATUS); // off
			in_magnet->setStatus(SEFL::ActuatorClientStatuses::ON_STATUS);

			SEFL::Logger::verbose("Elevatorout", "onManualFinished");
		}
	}

	void Elevatorout::onDefault()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();

			//		out_magnet->setStatus(SEFL::ActuatorClientStatuses::OFF_STATUS);
			//		in_magnet->setStatus(SEFL::ActuatorClientStatuses::ON_STATUS);

			SEFL::Logger::verbose("Elevatorout", "onDefault");
		}
	}

	uint8_t Elevatorout::getElevatorInSwith()
	{
		return elevatorInSwith;
	}

	void Elevatorout::setElevatorInSwith(uint8_t elevatorInSwith)
	{
		this->elevatorInSwith = elevatorInSwith;
	}

	uint8_t Elevatorout::getElevatorOutSwith()
	{
		return elevatorOutSwith;
	}

	void Elevatorout::setElevatorOutSwith(uint8_t elevatorOutSwith)
	{
		this->elevatorOutSwith = elevatorOutSwith;
	}

	DFRobotDFPlayerMini *Elevatorout::getPlayer()
	{
		return player;
	}

	uint8_t Elevatorout::getFlag()
	{
		return flag;
	}

	Magnet *Elevatorout::getOutMagnet()
	{
		return out_magnet;
	}

	void Elevatorout::setOutMagnet(Magnet *outMagnet)
	{
		out_magnet = outMagnet;
	}

	Magnet *Elevatorout::getInMagnet()
	{
		return in_magnet;
	}

	void Elevatorout::setInMagnet(Magnet *inMagnet)
	{
		in_magnet = inMagnet;
	}

	void Elevatorout::setFlag(uint8_t flag)
	{
		this->flag = flag;
	}

	void Elevatorout::setPlayer(DFRobotDFPlayerMini *player)
	{
		this->player = player;
	}

} /* namespace SEFL */

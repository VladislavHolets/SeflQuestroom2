/*
 * chopper.cpp
 *
 *  Created on: Jan 31, 2022
 *      Author: piatk
 */

#include <custom_clients/chopper.h>
#include <DFRobotDFPlayerMini.h>
#include <utils/logger.h>
#include <peripherals/multpinsextender.h>
#include <peripherals/pwmpinsextender.h>

bool starterfl = 0;
bool chargerflag = 0;
bool play_fl = 0;

namespace SEFL
{

	Chopper::Chopper(MQTTClient &mqtt, const char *name, uint8_t reset_status,
					 const char *placement, const char *in_topic, const char *out_topic,
					 SEFL::Language language) : Quest_Basic_Client(mqtt, name, reset_status, placement, in_topic,
																   out_topic, language)
	{

		chopperCharger = 0;
		chopperStarter = 2;
		chopperLight = 0;
		chopperAddressLed = 1;

		player = nullptr;
	}

	void Chopper::onActive()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("Chopper", "onActive");
			Mext.digitalRead(chopperCharger);
			pinMode(Mext.getCi(), INPUT_PULLUP);
			play_fl = 0;
			chargerflag = 0;
			starterfl = 0;
		}

		SEFL::Logger::verbose("Chopper", Mext.digitalRead(chopperCharger));

		if (!Mext.digitalRead(chopperCharger) && !chargerflag)
		{
			Pext.digitalWrite(chopperLight, false);

			if (player != nullptr)
			{
				player->play(5);
				//	player = nullptr;
				SEFL::Logger::verbose("Chopper", "0005.mp3");
				chargerflag = 1;
			}
		}
		// Mext.digitalRead(chopperStarter);
		//	pinMode(Mext.getCi(), INPUT_PULLUP);

		if (!Mext.digitalRead(chopperStarter) && chargerflag)
		{
			starterfl = 1;
			if (!play_fl)
			{
				player->play(1);
				play_fl = 1;
				SEFL::Logger::verbose("Chopperride", "0001.mp3");
			}
		}

		if (chargerflag && starterfl)
		{
			this->setStatus(BasicClientStatuses::FINISHED_STATUS);
		}
	}

	void Chopper::onFinished()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("Chopper", "onFinished");
		}
	}

	void Chopper::onManualFinished()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("Chopper", "onManualFinished");
		}
	}

	void Chopper::onDefault()
	{

		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("Chopper", "onDefault");
			Pext.digitalWrite(chopperLight, true);
			player->stop();
		}
	}

	uint8_t Chopper::getChopperAddressLed() const
	{
		return chopperAddressLed;
	}

	void Chopper::setChopperAddressLed(uint8_t chopperAddressLed)
	{
		this->chopperAddressLed = chopperAddressLed;
	}

	uint8_t Chopper::getChopperCharger() const
	{
		return chopperCharger;
	}

	void Chopper::setChopperCharger(uint8_t chopperCharger)
	{
		this->chopperCharger = chopperCharger;
	}

	// uint8_t Chopper::getChopperLight() const {
	//	return chopperLight;
	// }
	//
	// void Chopper::setChopperLight(uint8_t chopperLight) {
	//	this->chopperLight = chopperLight;
	// }

	DFRobotDFPlayerMini *Chopper::getPlayer() const
	{
		return player;
	}

	uint8_t Chopper::getChopperStarter() const
	{
		return chopperStarter;
	}

	void Chopper::setChopperStarter(uint8_t chopperStarter)
	{
		this->chopperStarter = chopperStarter;
	}

	void Chopper::setPlayer(DFRobotDFPlayerMini *player)
	{
		this->player = player;
	}

	Chopper::~Chopper()
	{
		// TODO Auto-generated destructor stub
	}

} /* namespace SEFL */

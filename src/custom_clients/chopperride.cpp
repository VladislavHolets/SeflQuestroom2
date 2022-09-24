/*
 * chopperride.cpp
 *
 *  Created on: Jan 31, 2022
 *      Author: piatk
 */

#include <custom_clients/chopperride.h>
#include <DFRobotDFPlayerMini.h>
#include <utils/logger.h>
#include <peripherals/multpinsextender.h>
#include <peripherals/pwmpinsextender.h>
#include <wiring_constants.h>
#include <wiring_digital.h>
#include <wiring_time.h>

bool starterflag = 1;
bool play_flag = 0;
uint8_t prevaction = 0;
uint8_t chopperway;
uint8_t actioncount;
uint8_t action;
// uint64_t timer;

namespace SEFL
{

	Chopperride::Chopperride(MQTTClient &mqtt, const char *name, uint8_t reset_status,
							 const char *placement, const char *in_topic, const char *out_topic,
							 SEFL::Language language) : Quest_Basic_Client(mqtt, name, reset_status, placement, in_topic,
																		   out_topic, language)
	{

		chopperStarter = 0;
		chopperLeftButton = 1;
		chopperRightButton = 2;
		chopperThrottle = 3;

		chopperLight = 0;
		chopperAddressLed = 1;
		ContactMagnet = 2;
		player = nullptr;
		timer = 0;
		flagThrottle = 0;
		timerThrottle = 0;
	}

	void Chopperride::onActive()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("Chopperride", "onActive");
			Pext.digitalWrite(chopperLight, false);
			play_flag = 0;
			starterflag = 1;
			actioncount = 0;
			flagThrottle = 0;
			timerThrottle = millis();
		}
		action = 0;
		Mext.digitalRead(chopperStarter);
		pinMode(Mext.getCi(), INPUT_PULLUP);

		if (!Mext.digitalRead(chopperStarter))
			starterflag = 1;

		if (starterflag)
		{

			if (!play_flag)
			{
				// player->play(1);
				play_flag = 1;
				// SEFL::Logger::verbose("Chopperride", "0001.mp3");
			}

			//		if (!Mext.digitalRead(chopperRightButton)) {
			//			SEFL::Logger::verbose("Chopperride", "chopperRightButton");
			//		}
			//		if (!Mext.digitalRead(chopperLeftButton)) {
			//			SEFL::Logger::verbose("Chopperride", "chopperLeftButton2");
			//		}

			SEFL::Logger::verbose("chopperThrottle",
								  Mext.digitalRead(chopperThrottle));
			//		SEFL::Logger::verbose("flagThrottle", flagThrottle);

			//-------------------------------------------------------------------------------
			if (Mext.digitalRead(chopperThrottle) && flagThrottle == 0 && millis() - timerThrottle > 300)
			{
				if (Mext.digitalRead(chopperRightButton) && Mext.digitalRead(chopperLeftButton))
				{
					action = 3;
					SEFL::Logger::verbose("Chopperride", "chopperStraightThrottle");
					flagThrottle = 1;
				}
				else if (!Mext.digitalRead(chopperRightButton))
				{
					action = 2;
					SEFL::Logger::verbose("Chopperride", "chopperRightThrottle");
					flagThrottle = 1;
				}
				else if (!Mext.digitalRead(chopperLeftButton))
				{
					action = 1;
					SEFL::Logger::verbose("Chopperride", "chopperLeftThrottle");
					flagThrottle = 1;
				}
			}
			else if (!Mext.digitalRead(chopperThrottle) && flagThrottle == 0)
			{
				timerThrottle = millis();
			}

			if (!Mext.digitalRead(chopperThrottle) && millis() - timerThrottle > 300)
			{
				flagThrottle = 0;
			}
			else if (Mext.digitalRead(chopperThrottle) && flagThrottle == 1)
			{
				timerThrottle = millis();
			}

			//	SEFL::Logger::verbose("Chopperride", action);

			if (prevaction != action && action != 0)
			{
				SEFL::Logger::verbose("action ", action);

				// actioncount++;

				//			if (actioncount > 15) {
				//				player->play(4);
				//				SEFL::Logger::verbose("Chopperride", "0004.mp3");
				//				play_flag = 0;
				//				actioncount = 0;
				//				//	starterflag = 0;
				//				chopperway = 0;
				//
				//			}

				if (action == 3 || action == 2 || action == 1)
				{
					SEFL::Logger::verbose("Chopperride", "0003.mp3");

					player->play(2);
				}

				////    1 = left, 2 = right, 3 = throttle
				//// 	 L R S L R S S
				////	 1 2 3 1 2 3 3

				if (chopperway == 0 && action == 1)
				{
					chopperway = 1;
					action = 0;
				}
				//			else if (chopperway == 0 && (action == 2 || action == 3))
				//				chopperway = 0;

				if (chopperway == 1 && action == 2)
				{
					chopperway = 2;
					action = 0;
				}
				else if (chopperway == 1 && action == 3)
					chopperway = 0;
				else if (chopperway == 1 && action == 1)
					chopperway = 1;

				if (chopperway == 2 && action == 3)
				{
					chopperway = 3;
					action = 0;
				}
				else if (chopperway == 2 && action == 2)
					chopperway = 0;
				else if (chopperway == 2 && action == 1)
					chopperway = 1;

				if (chopperway == 3 && action == 1)
				{
					chopperway = 4;
					action = 0;
				}
				else if (chopperway == 3 && (action == 2 || action == 3))
					chopperway = 0;

				if (chopperway == 4 && action == 2)
				{
					chopperway = 5;
					action = 0;
				}
				else if (chopperway == 4 && action == 3)
					chopperway = 0;
				else if (chopperway == 4 && action == 1)
					chopperway = 1;

				if (chopperway == 6 && action == 3)
				{
					chopperway = 7;
					action = 0;
				}
				else if (chopperway == 6 && action == 2)
					chopperway = 0;
				else if (chopperway == 6 && action == 1)
					chopperway = 1;

				if (chopperway == 5 && action == 3)
				{
					chopperway = 6;
					action = 0;
				}
				else if (chopperway == 5 && action == 2)
					chopperway = 0;
				else if (chopperway == 5 && action == 1)
					chopperway = 1;

				SEFL::Logger::verbose("action ", action);
				SEFL::Logger::verbose("chopperway  ", chopperway);
				SEFL::Logger::verbose("actioncount   ", actioncount);
			}
		}

		prevaction = action;

		if (chopperway == 7)
		{
			this->setStatus(BasicClientStatuses::FINISHED_STATUS);
			chopperway = 0;
			starterflag = 0;
		}
	}

	void Chopperride::onFinished()
	{

		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("Chopperride", "onFinished");
			Pext.digitalWrite(chopperLight, false);
			Pext.digitalWrite(ContactMagnet, false);
			timer = millis();
			if (player != nullptr)
			{
				player->play(4);
			}

			//	player->stop();
		}
		if (millis() - timer > 1000)
		{
			Pext.digitalWrite(ContactMagnet, true);
		}
		if (millis() - timer > 3000)
		{
			Pext.digitalWrite(ContactMagnet, false);
		}
		if (millis() - timer > 4000)
		{
			Pext.digitalWrite(ContactMagnet, true);
		}
		if (millis() - timer > 6000)
		{
			Pext.digitalWrite(ContactMagnet, false);
		}
		if (millis() - timer > 7000)
		{
			Pext.digitalWrite(ContactMagnet, true);
		}
	}

	void Chopperride::onManualFinished()
	{

		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("Chopperride", "onManualFinished");
			Pext.digitalWrite(chopperLight, false);
			Pext.digitalWrite(ContactMagnet, false);
			timer = millis();
			if (player != nullptr)
			{
				player->play(4);
			}
			//	player->stop();
		}

		if (millis() - timer > 1000)
		{
			Pext.digitalWrite(ContactMagnet, true);
		}
		if (millis() - timer > 3000)
		{
			Pext.digitalWrite(ContactMagnet, false);
		}
		if (millis() - timer > 4000)
		{
			Pext.digitalWrite(ContactMagnet, true);
		}
		if (millis() - timer > 6000)
		{
			Pext.digitalWrite(ContactMagnet, false);
		}
		if (millis() - timer > 7000)
		{
			Pext.digitalWrite(ContactMagnet, true);
		}
	}

	void Chopperride::onDefault()
	{

		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			Pext.digitalWrite(chopperLight, true);
			SEFL::Logger::verbose("Chopperride", "onDefault");
			Pext.digitalWrite(ContactMagnet, true);
			player->stop();
		}
	}

	uint8_t Chopperride::getChopperAddressLed() const
	{
		return chopperAddressLed;
	}

	void Chopperride::setChopperAddressLed(uint8_t chopperAddressLed)
	{
		this->chopperAddressLed = chopperAddressLed;
	}

	uint8_t Chopperride::getChopperLeftButton() const
	{
		return chopperLeftButton;
	}

	void Chopperride::setChopperLeftButton(uint8_t chopperLeftButton)
	{
		this->chopperLeftButton = chopperLeftButton;
	}

	uint8_t Chopperride::getChopperLight() const
	{
		return chopperLight;
	}

	void Chopperride::setChopperLight(uint8_t chopperLight)
	{
		this->chopperLight = chopperLight;
	}

	uint8_t Chopperride::getChopperRightButton() const
	{
		return chopperRightButton;
	}

	void Chopperride::setChopperRightButton(uint8_t chopperRightButton)
	{
		this->chopperRightButton = chopperRightButton;
	}

	uint8_t Chopperride::getChopperStarter() const
	{
		return chopperStarter;
	}

	void Chopperride::setChopperStarter(uint8_t chopperStarter)
	{
		this->chopperStarter = chopperStarter;
	}

	uint8_t Chopperride::getChopperThrottle() const
	{
		return chopperThrottle;
	}

	void Chopperride::setChopperThrottle(uint8_t chopperThrottle)
	{
		this->chopperThrottle = chopperThrottle;
	}

	uint8_t Chopperride::getContactMagnet() const
	{
		return ContactMagnet;
	}

	void Chopperride::setContactMagnet(uint8_t contactMagnet)
	{
		ContactMagnet = contactMagnet;
	}

	DFRobotDFPlayerMini *Chopperride::getPlayer() const
	{
		return player;
	}

	uint64_t Chopperride::getTimer() const
	{
		return timer;
	}

	void Chopperride::setTimer(uint64_t timer)
	{
		this->timer = timer;
	}

	uint8_t Chopperride::getFlagThrottle() const
	{
		return flagThrottle;
	}

	void Chopperride::setFlagThrottle(uint8_t flagThrottle)
	{
		this->flagThrottle = flagThrottle;
	}

	uint64_t Chopperride::getTimerThrottle() const
	{
		return timerThrottle;
	}

	void Chopperride::setTimerThrottle(uint64_t timerThrottle)
	{
		this->timerThrottle = timerThrottle;
	}

	void Chopperride::setPlayer(DFRobotDFPlayerMini *player)
	{
		this->player = player;
	}

	Chopperride::~Chopperride()
	{
		// TODO Auto-generated destructor stub
	}

} /* namespace SEFL */

/*
 * sportbike.cpp
 *
 *  Created on: Jan 31, 2022
 *      Author: piatk
 */

#include "DFRobotDFPlayerMini.h"
#include "sportbike.h"
#include <wiring_constants.h>
#include <wiring_digital.h>
#include <wiring_time.h>

int thtrottle_count;

bool flag = 0;
//<<<<<<< HEAD
// int thtrottle_count;
//
//=======
//>>>>>>> parent of 5b9d42d (sportBike)
namespace SEFL
{

	Sportbike::Sportbike(MQTTClient &mqtt, const char *name, uint8_t reset_status,
						 const char *placement, const char *in_topic, const char *out_topic,
						 SEFL::Language language) : Quest_Basic_Client(mqtt, name, reset_status, placement, in_topic,
																	   out_topic, language)
	{

		sportbikeKey = 0;
		sportbikeLeftButton = 1;
		sportbikeRightButton = 2;
		sportbikeThrottle = 3;
		sportbikeRideLightButton = 4;

		sportbikeLeftRelay = 0;
		sportbikeRightRelay = 1;
		sportbikeRideLight = 2;
		sportbikePowerRelay = 3;
		sportbikeRightRelayFront = 4;
		sportbikeLeftRelayFront = 5;
		dashboardLight = 6;
		tachometerPower = 7;

		player = nullptr;
		timing = 0;
		timerForTrottle = 0;
		timerForTachometer = 0;
		flagTachometer = 0;
		flagThrottle = 0;
		flagFinal = 0;
		timerFinal = 0;
	}

	//<<<<<<< HEAD

	void Sportbike::onActive()
	{

		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("Sportbike", "onActive");
			thtrottle_count = 0;
			flag = 0;
			if (player != nullptr)
			{
				player->volume(30);
				player->play(7);
			}
			flagFinal = 0;
			timerFinal = 0;
			timerForTachometer = millis();
		}

		Mext.digitalRead(sportbikeKey);
		pinMode(Mext.getCi(), INPUT_PULLUP);

		//---------------- TURN SIGNALS-----------------------

		// SEFL::Logger::verbose("Sportbike key",Mext.digitalRead(sportbikeKey));
		//	SEFL::Logger::verbose("Sportbike left butt",Mext.digitalRead(sportbikeLeftButton));

		//	SEFL::Logger::verbose("left", Mext.digitalRead(sportbikeLeftButton));
		//	SEFL::Logger::verbose("Right", Mext.digitalRead(sportbikeRightButton));

		// Pext.digitalWrite(sportbikeLeftRelay, Mext.digitalRead(sportbikeLeftButton));
		if (Mext.digitalRead(sportbikeLeftButton) && !Mext.digitalRead(sportbikeRightButton) && !Mext.digitalRead(sportbikeKey))
		{
			if (millis() - timing < 500)
			{
				Pext.digitalWrite(sportbikeLeftRelay, HIGH);
				Pext.digitalWrite(sportbikeLeftRelayFront, HIGH);
			}
			else if (millis() - timing < 1000)
			{
				Pext.digitalWrite(sportbikeLeftRelay, LOW);
				Pext.digitalWrite(sportbikeLeftRelayFront, LOW);
			}
			else
			{
				timing = millis();
			}
		}
		else
		{
			Pext.digitalWrite(sportbikeLeftRelay, HIGH);
			Pext.digitalWrite(sportbikeLeftRelayFront, HIGH);
		}

		//	SEFL::Logger::verbose("Sportbike right butt",Mext.digitalRead(sportbikeRightButton));
		// Pext.digitalWrite(sportbikeRightRelay, Mext.digitalRead(sportbikeRightButton));

		if (Mext.digitalRead(sportbikeRightButton) && !Mext.digitalRead(sportbikeLeftButton) && !Mext.digitalRead(sportbikeKey))
		{
			if (millis() - timing < 500)
			{
				Pext.digitalWrite(sportbikeRightRelay, HIGH);
				Pext.digitalWrite(sportbikeRightRelayFront, HIGH);
			}
			else if (millis() - timing < 1000)
			{
				Pext.digitalWrite(sportbikeRightRelay, LOW);
				Pext.digitalWrite(sportbikeRightRelayFront, LOW);
			}
			else
			{
				timing = millis();
			}
		}
		else
		{
			Pext.digitalWrite(sportbikeRightRelay, HIGH);
			Pext.digitalWrite(sportbikeRightRelayFront, HIGH);
		}

		//----------------FRONT LIGHT-------------------
		if (!Mext.digitalRead(sportbikeRideLightButton) && !Mext.digitalRead(sportbikeKey))
		{
			Pext.digitalWrite(sportbikeRideLight, LOW);
		}
		else
		{
			Pext.digitalWrite(sportbikeRideLight, HIGH);
		}
		// Pext.digitalWrite(sportbikeRideLight, Mext.digitalRead(sportbikeRideLightButton));

		//-------------------------sportbikeThrottle-------------------------
		if (!Mext.digitalRead(sportbikeKey))
		{

			Pext.digitalWrite(dashboardLight, LOW);
			Pext.digitalWrite(sportbikePowerRelay, LOW);

			if (!flag)
			{
				SEFL::Logger::verbose("Sportbike", "key");
				if (player != nullptr)
				{
					player->play(1);
					SEFL::Logger::verbose("Sportbike", "0001.mp3");
				}
				flag = 1;
				thtrottle_count = 1;
				flagTachometer = 0;
				flagThrottle = 0;
			}

			//-------------------------------------------------------------------
			//--------------------THOTTLE ONE------------------------------------
			if (thtrottle_count == 1)
			{
				if (flagTachometer == 0 && millis() - timerForTachometer > 3000)
				{
					Pext.analogWrite(tachometerPower, 4096);
					timerForTachometer = millis();
					flagTachometer = 1;
				}
				if (flagTachometer == 1 && millis() - timerForTachometer > 500)
				{
					Pext.analogWrite(tachometerPower, 0);
					timerForTachometer = millis();
					flagTachometer = 2;
				}
				if (flagTachometer == 2 && millis() - timerForTachometer > 300)
				{
					Pext.analogWrite(tachometerPower, 4096);
					timerForTachometer = millis();
					flagTachometer = 3;
				}
				if (flagTachometer == 3 && millis() - timerForTachometer > 500)
				{
					Pext.analogWrite(tachometerPower, 0);
					timerForTachometer = millis();
					flagTachometer = 0;
				}
			}

			if (thtrottle_count == 1)
			{
				if (!Mext.digitalRead(sportbikeThrottle) && flagThrottle == 0)
				{
					timerForTrottle = millis();
					flagThrottle = 1;
				}
				if (Mext.digitalRead(sportbikeThrottle) && flagThrottle == 1 && millis() - timerForTrottle > 100)
				{
					timerForTrottle = millis();
					flagThrottle = 2;
				}
				if (!Mext.digitalRead(sportbikeThrottle) && flagThrottle == 2 && millis() - timerForTrottle > 100)
				{
					thtrottle_count = 2;
					flagTachometer = 0;
					if (player != nullptr)
					{
						player->play(3);
						SEFL::Logger::verbose("Sportbike", "0003.mp3");
					}
					flagThrottle = 0;
				}
				if (millis() - timerForTrottle > 1000)
				{
					flagThrottle = 0;
				}
			}
			//--------------------THOTTLE TWO------------------------------------
			if (thtrottle_count == 2)
			{
				if (flagTachometer == 0 && millis() - timerForTachometer > 3000)
				{
					Pext.analogWrite(tachometerPower, 4096);
					timerForTachometer = millis();
					flagTachometer = 1;
				}
				if (flagTachometer == 1 && millis() - timerForTachometer > 500)
				{
					Pext.analogWrite(tachometerPower, 0);
					timerForTachometer = millis();
					flagTachometer = 0;
				}
			}

			if (thtrottle_count == 2)
			{

				if (Mext.digitalRead(sportbikeThrottle) && flagThrottle == 0 && millis() - timerForTrottle > 100)
				{
					timerForTrottle = millis();
					flagThrottle = 1;
				}
				if (!Mext.digitalRead(sportbikeThrottle) && flagThrottle == 1 && millis() - timerForTrottle > 100)
				{
					thtrottle_count = 3;
					flagTachometer = 0;
					if (player != nullptr)
					{
						player->play(2);
						SEFL::Logger::verbose("Sportbike", "0002.mp3");
					}
					flagThrottle = 0;
				}
				if (millis() - timerForTrottle > 1000)
				{
					flagThrottle = 0;
				}
			}
			//--------------------THOTTLE THREE------------------------------------
			if (thtrottle_count == 3)
			{
				if (flagTachometer == 0 && millis() - timerForTachometer > 3000)
				{
					Pext.analogWrite(tachometerPower, 4096);
					timerForTachometer = millis();
					flagTachometer = 1;
				}
				if (flagTachometer == 1 && millis() - timerForTachometer > 500)
				{
					Pext.analogWrite(tachometerPower, 0);
					timerForTachometer = millis();
					flagTachometer = 2;
				}
				if (flagTachometer == 2 && millis() - timerForTachometer > 300)
				{
					Pext.analogWrite(tachometerPower, 4096);
					timerForTachometer = millis();
					flagTachometer = 3;
				}
				if (flagTachometer == 3 && millis() - timerForTachometer > 500)
				{
					Pext.analogWrite(tachometerPower, 0);
					timerForTachometer = millis();
					flagTachometer = 4;
				}
				if (flagTachometer == 4 && millis() - timerForTachometer > 300)
				{
					Pext.analogWrite(tachometerPower, 4096);
					timerForTachometer = millis();
					flagTachometer = 5;
				}
				if (flagTachometer == 5 && millis() - timerForTachometer > 500)
				{
					Pext.analogWrite(tachometerPower, 0);
					timerForTachometer = millis();
					flagTachometer = 0;
				}
			}

			if (thtrottle_count == 3)
			{

				if (Mext.digitalRead(sportbikeThrottle) && flagThrottle == 0 && millis() - timerForTrottle > 100)
				{
					timerForTrottle = millis();
					flagThrottle = 1;
				}
				if (!Mext.digitalRead(sportbikeThrottle) && flagThrottle == 1 && millis() - timerForTrottle > 100)
				{
					timerForTrottle = millis();
					flagThrottle = 2;
				}

				if (Mext.digitalRead(sportbikeThrottle) && flagThrottle == 2 && millis() - timerForTrottle > 100)
				{
					timerForTrottle = millis();
					flagThrottle = 3;
				}
				if (!Mext.digitalRead(sportbikeThrottle) && flagThrottle == 3 && millis() - timerForTrottle > 100)
				{
					timerForTrottle = millis();
					flagThrottle = 4;
				}

				if (Mext.digitalRead(sportbikeThrottle) && flagThrottle == 4 && millis() - timerForTrottle > 100)
				{
					timerForTrottle = millis();
					flagThrottle = 5;
				}
				if (!Mext.digitalRead(sportbikeThrottle) && flagThrottle == 5 && millis() - timerForTrottle > 100)
				{
					thtrottle_count = 4;
					flagTachometer = 0;
					if (player != nullptr)
					{
						player->play(5);
						SEFL::Logger::verbose("Sportbike", "0005.mp3");
					}
				}

				if (millis() - timerForTrottle > 1000)
				{
					flagThrottle = 0;
				}
			}

			//		if (thtrottle_count==1
			//				{
			//			        if (player != nullptr) {
			//					player->play(2);
			//					SEFL::Logger::verbose("Sportbike","0002.mp3");
			//					thtrottle_count = 2;
			//					}
			//			}
			//
			//		if (thtrottle_count==2
			//					&& Mext.digitalRead(sportbikeThrottle)){
			//					thtrottle_count = 3;
			//				}
			//
			//
			//		if (thtrottle_count==3
			//				&& !Mext.digitalRead(sportbikeThrottle)){
			//				if (player != nullptr) {
			//						player->play(3);
			//						SEFL::Logger::verbose("Sportbike","0003.mp3");
			//						thtrottle_count = 4;
			//						}
			//				}
			//
			//
			//		if (thtrottle_count==4
			//						&& Mext.digitalRead(sportbikeThrottle)){
			//						thtrottle_count = 5;
			//					}
			//
			//
			//		if (thtrottle_count==5
			//				&& !Mext.digitalRead(sportbikeThrottle)){
			//					if (player != nullptr) {
			//							player->play(5);
			//							SEFL::Logger::verbose("Sportbike","0005.mp3");
			//							thtrottle_count = 6;
			//							}
			//					}

			if (thtrottle_count == 4)
			{

				//    	 if (player != nullptr) {
				//    	 						player->play(5);
				//    	 						SEFL::Logger::verbose("Sportbike","0005.mp3");
				//    	 						thtrottle_count = 5;
				//    	 						}
				if (flagFinal == 0)
				{
					flagFinal = 1;
					timerFinal = millis();
				}
				if (flagFinal == 1 && millis() - timerFinal > 5000)
				{
					this->setStatus(BasicClientStatuses::FINISHED_STATUS);
				}
			}
		}
		else
		{
			Pext.digitalWrite(dashboardLight, HIGH);
			Pext.digitalWrite(sportbikePowerRelay, HIGH);
			Pext.analogWrite(tachometerPower, 0);

			if (flag != 0 && player != nullptr) 
			{
				player->play(6);
			}

			thtrottle_count = 0;
			flag = 0;
		}
	}

	void Sportbike::onFinished()
	{

		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			if (player != nullptr)
			{
				player->stop();
			}
			SEFL::Logger::verbose("Sportbike", "onFinished");
			Pext.digitalWrite(sportbikeRideLight, HIGH);
			Pext.digitalWrite(sportbikePowerRelay, HIGH);

			Pext.digitalWrite(sportbikeLeftRelay, HIGH);
			Pext.digitalWrite(sportbikeLeftRelayFront, HIGH);
			Pext.digitalWrite(sportbikeRightRelay, HIGH);
			Pext.digitalWrite(sportbikeRightRelayFront, HIGH);

			Pext.digitalWrite(dashboardLight, HIGH);
			Pext.analogWrite(tachometerPower, 0);
		}
	}

	void Sportbike::onManualFinished()
	{

		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			if (player != nullptr)
			{
				player->stop();
			}
			SEFL::Logger::verbose("Sportbike", "onManualFinished");
			Pext.digitalWrite(sportbikeRideLight, HIGH);
			Pext.digitalWrite(sportbikePowerRelay, HIGH);

			Pext.digitalWrite(sportbikeLeftRelay, HIGH);
			Pext.digitalWrite(sportbikeLeftRelayFront, HIGH);
			Pext.digitalWrite(sportbikeRightRelay, HIGH);
			Pext.digitalWrite(sportbikeRightRelayFront, HIGH);

			Pext.digitalWrite(dashboardLight, HIGH);
			Pext.analogWrite(tachometerPower, 0);
		}
	}

	void Sportbike::onDefault()
	{

		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("Sportbike", "onDefault");
			if (player != nullptr)
			{
				player->stop();
			}
			Pext.digitalWrite(sportbikeRideLight, HIGH);
			Pext.digitalWrite(sportbikePowerRelay, HIGH);

			Pext.digitalWrite(sportbikeLeftRelay, HIGH);
			Pext.digitalWrite(sportbikeLeftRelayFront, HIGH);
			Pext.digitalWrite(sportbikeRightRelay, HIGH);
			Pext.digitalWrite(sportbikeRightRelayFront, HIGH);

			Pext.digitalWrite(dashboardLight, HIGH);
			Pext.analogWrite(tachometerPower, 0);
		}
	}

	DFRobotDFPlayerMini *Sportbike::getPlayer()
	{
		return player;
	}

	void Sportbike::setPlayer(DFRobotDFPlayerMini *player)
	{
		this->player = player;
	}

	uint8_t Sportbike::getSportbikeKey()
	{
		return sportbikeKey;
	}

	void Sportbike::setSportbikeKey(uint8_t sportbikeKey)
	{
		this->sportbikeKey = sportbikeKey;
	}

	uint8_t Sportbike::getSportbikeLeftButton()
	{
		return sportbikeLeftButton;
	}

	void Sportbike::setSportbikeLeftButton(uint8_t sportbikeLeftButton)
	{
		this->sportbikeLeftButton = sportbikeLeftButton;
	}

	uint8_t Sportbike::getSportbikeRideLight()
	{
		return sportbikeRideLight;
	}

	void Sportbike::setSportbikeRideLight(uint8_t sportbikeRideLight)
	{
		this->sportbikeRideLight = sportbikeRideLight;
	}

	uint8_t Sportbike::getSportbikeRightButton()
	{
		return sportbikeRightButton;
	}

	void Sportbike::setSportbikeRightButton(uint8_t sportbikeRightButton)
	{
		this->sportbikeRightButton = sportbikeRightButton;
	}

	uint8_t Sportbike::getSportbikeThrottle()
	{
		return sportbikeThrottle;
	}

	uint8_t Sportbike::getSportbikeLeftRelay()
	{
		return sportbikeLeftRelay;
	}

	void Sportbike::setSportbikeLeftRelay(uint8_t sportbikeLeftRelay)
	{
		this->sportbikeLeftRelay = sportbikeLeftRelay;
	}

	uint8_t Sportbike::getSportbikeRightRelay()
	{
		return sportbikeRightRelay;
	}

	uint8_t Sportbike::getSportbikePowerRelay()
	{
		return sportbikePowerRelay;
	}

	uint8_t Sportbike::getSportbikeLeftRelayFront()
	{
		return sportbikeLeftRelayFront;
	}

	void Sportbike::setSportbikeLeftRelayFront(uint8_t sportbikeLeftRelayFront)
	{
		this->sportbikeLeftRelayFront = sportbikeLeftRelayFront;
	}

	uint8_t Sportbike::getSportbikeRightRelayFront()
	{
		return sportbikeRightRelayFront;
	}

	uint8_t Sportbike::getDashboardLight()
	{
		return dashboardLight;
	}

	uint8_t Sportbike::getTachometerPower()
	{
		return tachometerPower;
	}

	uint64_t Sportbike::getTimerForTrottle()
	{
		return timerForTrottle;
	}

	void Sportbike::setTimerForTrottle(uint64_t timerForTrottle)
	{
		this->timerForTrottle = timerForTrottle;
	}

	uint64_t Sportbike::getTimerForTachometer()
	{
		return timerForTachometer;
	}

	void Sportbike::setTimerForTachometer(uint64_t timerForTachometer)
	{
		this->timerForTachometer = timerForTachometer;
	}

	uint8_t Sportbike::getFlagTachometer()
	{
		return flagTachometer;
	}

	uint8_t Sportbike::getFlagThrottle()
	{
		return flagThrottle;
	}

	uint8_t Sportbike::getFlagFinal()
	{
		return flagFinal;
	}

	void Sportbike::setFlagFinal(uint8_t flagFinal)
	{
		this->flagFinal = flagFinal;
	}

	uint64_t Sportbike::getTimerFinal()
	{
		return timerFinal;
	}

	void Sportbike::setTimerFinal(uint64_t timerFinal)
	{
		this->timerFinal = timerFinal;
	}

	void Sportbike::setFlagThrottle(uint8_t flagThrottle)
	{
		this->flagThrottle = flagThrottle;
	}

	void Sportbike::setFlagTachometer(uint8_t flagTachometer)
	{
		this->flagTachometer = flagTachometer;
	}

	void Sportbike::setTachometerPower(uint8_t tachometerPower)
	{
		this->tachometerPower = tachometerPower;
	}

	void Sportbike::setDashboardLight(uint8_t dashboardLight)
	{
		this->dashboardLight = dashboardLight;
	}

	void Sportbike::setSportbikeRightRelayFront(uint8_t sportbikeRightRelayFront)
	{
		this->sportbikeRightRelayFront = sportbikeRightRelayFront;
	}

	void Sportbike::setSportbikePowerRelay(uint8_t sportbikePowerRelay)
	{
		this->sportbikePowerRelay = sportbikePowerRelay;
	}

	void Sportbike::setSportbikeRightRelay(uint8_t sportbikeRightRelay)
	{
		this->sportbikeRightRelay = sportbikeRightRelay;
	}

	void Sportbike::setSportbikeThrottle(uint8_t sportbikeThrottle)
	{
		this->sportbikeThrottle = sportbikeThrottle;
	}

	Sportbike::~Sportbike()
	{
		// TODO Auto-generated destructor stub
	}

} /* namespace SEFL */

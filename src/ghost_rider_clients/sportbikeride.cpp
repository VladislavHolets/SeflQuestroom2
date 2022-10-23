/*
 * sportbikeride.cpp
 *
 *  Created on: Jan 31, 2022
 *      Author: piatk
 */

#include "DFRobotDFPlayerMini.h"
#include "PCA9685.h"
#include "sportbikeride.h"
#include <wiring_constants.h>
#include <wiring_time.h>

class PCA9685;

//#include "PCA9685.h"
////PCA9685 pwmController(Wire1);
// PCA9685_ServoEval pwmServo1;

PCA9685_ServoEval pwmServo1;

namespace SEFL
{

	Sportbikeride::Sportbikeride(MQTTClient &mqtt, const char *name, uint8_t reset_status,
								 const char *placement, const char *in_topic, const char *out_topic,
								 SEFL::Language language) : Quest_Basic_Client(mqtt, name, reset_status, placement, in_topic,
																			   out_topic, language)
	{

		sportbikeRideThrottle = 0;
		sportbikeRideLight = 1;
		sportbikeServo = 2;
		sportbikeLeftRelay = 3;
		sportbikeRightRelay = 4;
		dashboardLight = 5;
		sportbikePowerRelay = 6;
		sportbikeRightRelayFront = 7;
		sportbikeLeftRelayFront = 8;
		tachometerPower = 9;
		sportbikePowerServo = 10;

		countTurn = 0;

		timer = 0;
		timerForTurnSignals = 0;
		turnSignalsFlag = 0;
		throttleFlag = 0;

		player = nullptr;
		pwmController = nullptr;
	}

	void Sportbikeride::onActive()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("Sportbikeride", "onActive");
			countTurn = 0;
			throttleFlag = 1;
			timer = millis();
			timerForTurnSignals = millis();
			Pext.getHandler()->setPWMFrequency(50);

			if (player != nullptr)
			{
				player->volume(30);
				player->play(1);
				SEFL::Logger::verbose("Sportbikeride", "0001.mp3");
			}
			Pext.getHandler()->setChannelPWM(sportbikeServo,
											 pwmServo1.pwmForAngle(0));

			Pext.digitalWrite(dashboardLight, LOW);
			Pext.digitalWrite(sportbikePowerRelay, LOW);
			// Pext.digitalWrite(sportbikeRideLight, LOW);
			Pext.digitalWrite(sportbikePowerServo, LOW);
		}

		//------------------------------------------------------
		if (countTurn == 0 && millis() - timer > 7000)
		{
			// Pext.analogWrite(sportbikeServo, 1000);
			Pext.getHandler()->setChannelPWM(sportbikeServo,
											 pwmServo1.pwmForAngle(-43));
			turnSignalsFlag = 1;
			throttleFlag = 0;
			countTurn = 1;
			timer = millis();
			Pext.digitalWrite(sportbikeRideLight, HIGH);
		}

		if (countTurn == 1 && millis() - timer > 3000)
		{
			// Pext.analogWrite(sportbikeServo, 1000);
			Pext.getHandler()->setChannelPWM(sportbikeServo,
											 pwmServo1.pwmForAngle(37));
			throttleFlag = 0;
			turnSignalsFlag = 2;
			countTurn = 2;
			timer = millis();
		}
		if (countTurn == 2 && millis() - timer > 3000)
		{
			// Pext.analogWrite(sportbikeServo, 1000);
			Pext.getHandler()->setChannelPWM(sportbikeServo,
											 pwmServo1.pwmForAngle(-3));
			throttleFlag = 0;
			turnSignalsFlag = 0;
			countTurn = 3;
			timer = millis();
			Pext.digitalWrite(sportbikeRideLight, LOW);
		}
		if (countTurn == 3 && millis() - timer > 3000)
		{
			// Pext.analogWrite(sportbikeServo, 1000);
			Pext.getHandler()->setChannelPWM(sportbikeServo,
											 pwmServo1.pwmForAngle(-43));
			throttleFlag = 0;
			turnSignalsFlag = 1;
			countTurn = 4;
			timer = millis();
			Pext.digitalWrite(sportbikeRideLight, HIGH);
		}
		if (countTurn == 4 && millis() - timer > 3000)
		{
			// Pext.analogWrite(sportbikeServo, 1000);
			Pext.getHandler()->setChannelPWM(sportbikeServo,
											 pwmServo1.pwmForAngle(37));
			throttleFlag = 0;
			turnSignalsFlag = 2;
			countTurn = 5;
			timer = millis();
		}
		if (countTurn == 5 && millis() - timer > 3000)
		{
			// Pext.analogWrite(sportbikeServo, 1000);
			Pext.getHandler()->setChannelPWM(sportbikeServo,
											 pwmServo1.pwmForAngle(-3));
			throttleFlag = 0;
			turnSignalsFlag = 0;
			countTurn = 6;
			timer = millis();
			Pext.digitalWrite(sportbikeRideLight, LOW);
		}
		if (countTurn == 6 && millis() - timer > 2000)
		{
			// Pext.analogWrite(sportbikeServo, 1000);
			// Pext.getHandler()->setChannelPWM(sportbikeServo,
			// pwmServo1.pwmForAngle(-2));
			throttleFlag = 0;
			turnSignalsFlag = 0;
			countTurn = 0;
			timer = millis();
		}

		if (player != nullptr && !throttleFlag && millis() - timer > 1000)
		{
			player->play(2);
			SEFL::Logger::verbose("Sportbikeride", "0002.mp3");
			throttleFlag = 1;
			Pext.analogWrite(tachometerPower, 4096);
		}
		if (throttleFlag && millis() - timer > 1500)
		{
			Pext.analogWrite(tachometerPower, 0);
		}
		//------------------TURN SIGNALS------------------------

		if (turnSignalsFlag == 1 && millis() - timer > 500)
		{
			if (millis() - timerForTurnSignals < 500)
			{
				Pext.digitalWrite(sportbikeLeftRelay, HIGH);
				Pext.digitalWrite(sportbikeLeftRelayFront, HIGH);
			}
			else if (millis() - timerForTurnSignals < 1000)
			{
				Pext.digitalWrite(sportbikeLeftRelay, LOW);
				Pext.digitalWrite(sportbikeLeftRelayFront, LOW);
			}
			else
			{
				timerForTurnSignals = millis();
			}
		}
		else
		{
			Pext.digitalWrite(sportbikeLeftRelay, HIGH);
			Pext.digitalWrite(sportbikeLeftRelayFront, HIGH);
		}

		if (turnSignalsFlag == 2 && millis() - timer > 500)
		{
			if (millis() - timerForTurnSignals < 500)
			{
				Pext.digitalWrite(sportbikeRightRelay, HIGH);
				Pext.digitalWrite(sportbikeRightRelayFront, HIGH);
			}
			else if (millis() - timerForTurnSignals < 1000)
			{
				Pext.digitalWrite(sportbikeRightRelay, LOW);
				Pext.digitalWrite(sportbikeRightRelayFront, LOW);
			}
			else
			{
				timerForTurnSignals = millis();
			}
		}
		else
		{
			Pext.digitalWrite(sportbikeRightRelay, HIGH);
			Pext.digitalWrite(sportbikeRightRelayFront, HIGH);
		}
	}

	void Sportbikeride::onFinished()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("Sportbikeride", "onFinished");
			player->stop();

			Pext.digitalWrite(dashboardLight, HIGH);
			Pext.digitalWrite(sportbikePowerRelay, HIGH);
			Pext.digitalWrite(sportbikeRideLight, HIGH);
			Pext.digitalWrite(sportbikePowerServo, HIGH);

			Pext.digitalWrite(sportbikeLeftRelay, HIGH);
			Pext.digitalWrite(sportbikeLeftRelayFront, HIGH);
			Pext.digitalWrite(sportbikeRightRelay, HIGH);
			Pext.digitalWrite(sportbikeRightRelayFront, HIGH);

			Pext.analogWrite(tachometerPower, 0);
		}
	}

	void Sportbikeride::onManualFinished()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("Sportbikeride", "onManualFinished");
			player->stop();

			Pext.digitalWrite(dashboardLight, HIGH);
			Pext.digitalWrite(sportbikePowerRelay, HIGH);
			Pext.digitalWrite(sportbikeRideLight, HIGH);
			Pext.digitalWrite(sportbikePowerServo, HIGH);

			Pext.digitalWrite(sportbikeLeftRelay, HIGH);
			Pext.digitalWrite(sportbikeLeftRelayFront, HIGH);
			Pext.digitalWrite(sportbikeRightRelay, HIGH);
			Pext.digitalWrite(sportbikeRightRelayFront, HIGH);

			Pext.analogWrite(tachometerPower, 0);
		}
	}

	void Sportbikeride::onDefault()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("Sportbikeride", "onDefault");
			player->stop();

			Pext.digitalWrite(dashboardLight, HIGH);
			Pext.digitalWrite(sportbikePowerRelay, HIGH);
			Pext.digitalWrite(sportbikeRideLight, HIGH);
			Pext.digitalWrite(sportbikePowerServo, HIGH);

			Pext.digitalWrite(sportbikeLeftRelay, HIGH);
			Pext.digitalWrite(sportbikeLeftRelayFront, HIGH);
			Pext.digitalWrite(sportbikeRightRelay, HIGH);
			Pext.digitalWrite(sportbikeRightRelayFront, HIGH);

			Pext.analogWrite(tachometerPower, 0);
		}
	}

	DFRobotDFPlayerMini *Sportbikeride::getPlayer()
	{
		return player;
	}

	void Sportbikeride::setPlayer(DFRobotDFPlayerMini *player)
	{
		this->player = player;
	}

	uint8_t Sportbikeride::getSportbikeRideLight()
	{
		return sportbikeRideLight;
	}

	void Sportbikeride::setSportbikeRideLight(uint8_t sportbikeRideLight)
	{
		this->sportbikeRideLight = sportbikeRideLight;
	}

	uint8_t Sportbikeride::getSportbikeRideThrottle()
	{
		return sportbikeRideThrottle;
	}

	void Sportbikeride::setSportbikeRideThrottle(uint8_t sportbikeRideThrottle)
	{
		this->sportbikeRideThrottle = sportbikeRideThrottle;
	}

	uint8_t Sportbikeride::getSportbikeServo()
	{
		return sportbikeServo;
	}

	uint8_t Sportbikeride::getSportbikeLeftRelay()
	{
		return sportbikeLeftRelay;
	}

	void Sportbikeride::setSportbikeLeftRelay(uint8_t sportbikeLeftRelay)
	{
		this->sportbikeLeftRelay = sportbikeLeftRelay;
	}

	uint8_t Sportbikeride::getSportbikeRightRelay()
	{
		return sportbikeRightRelay;
	}

	PCA9685 *&Sportbikeride::getPwmController()
	{
		return pwmController;
	}

	uint8_t Sportbikeride::getDashboardLight()
	{
		return dashboardLight;
	}

	void Sportbikeride::setDashboardLight(uint8_t dashboardLight)
	{
		this->dashboardLight = dashboardLight;
	}

	uint8_t Sportbikeride::getSportbikeLeftRelayFront()
	{
		return sportbikeLeftRelayFront;
	}

	void Sportbikeride::setSportbikeLeftRelayFront(
		uint8_t sportbikeLeftRelayFront)
	{
		this->sportbikeLeftRelayFront = sportbikeLeftRelayFront;
	}

	uint8_t Sportbikeride::getSportbikePowerRelay()
	{
		return sportbikePowerRelay;
	}

	void Sportbikeride::setSportbikePowerRelay(uint8_t sportbikePowerRelay)
	{
		this->sportbikePowerRelay = sportbikePowerRelay;
	}

	uint8_t Sportbikeride::getSportbikeRightRelayFront()
	{
		return sportbikeRightRelayFront;
	}

	void Sportbikeride::setSportbikeRightRelayFront(
		uint8_t sportbikeRightRelayFront)
	{
		this->sportbikeRightRelayFront = sportbikeRightRelayFront;
	}

	uint8_t Sportbikeride::getTachometerPower()
	{
		return tachometerPower;
	}

	uint8_t Sportbikeride::getSportbikePowerServo()
	{
		return sportbikePowerServo;
	}

	uint8_t Sportbikeride::getCountTurn()
	{
		return countTurn;
	}

	uint64_t Sportbikeride::getTimerForTurnSignals()
	{
		return timerForTurnSignals;
	}

	void Sportbikeride::setTimerForTurnSignals(uint64_t timerForTurnSignals)
	{
		this->timerForTurnSignals = timerForTurnSignals;
	}

	uint8_t Sportbikeride::getTurnSignalsFlag()
	{
		return turnSignalsFlag;
	}

	uint8_t Sportbikeride::getThrottleFlag()
	{
		return throttleFlag;
	}

	void Sportbikeride::setThrottleFlag(uint8_t throttleFlag)
	{
		this->throttleFlag = throttleFlag;
	}

	void Sportbikeride::setTurnSignalsFlag(uint8_t turnSignalsFlag)
	{
		this->turnSignalsFlag = turnSignalsFlag;
	}

	void Sportbikeride::setCountTurn(uint8_t countTurn)
	{
		this->countTurn = countTurn;
	}

	uint64_t Sportbikeride::getTimer()
	{
		return timer;
	}

	void Sportbikeride::setTimer(uint64_t timer)
	{
		this->timer = timer;
	}

	void Sportbikeride::setSportbikePowerServo(uint8_t sportbikePowerServo)
	{
		this->sportbikePowerServo = sportbikePowerServo;
	}

	void Sportbikeride::setTachometerPower(uint8_t tachometerPower)
	{
		this->tachometerPower = tachometerPower;
	}

	void Sportbikeride::setPwmController(PCA9685 *&pwmController)
	{
		this->pwmController = pwmController;
	}

	void Sportbikeride::setSportbikeRightRelay(uint8_t sportbikeRightRelay)
	{
		this->sportbikeRightRelay = sportbikeRightRelay;
	}

	void Sportbikeride::setSportbikeServo(uint8_t sportbikeServo)
	{
		this->sportbikeServo = sportbikeServo;
	}

	Sportbikeride::~Sportbikeride()
	{
		// TODO Auto-generated destructor stub
	}

} /* namespace SEFL */

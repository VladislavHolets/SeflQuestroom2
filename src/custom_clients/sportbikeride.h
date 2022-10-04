/*
 * sportbikeride.h
 *
 *  Created on: Jan 31, 2022
 *      Author: piatk
 */

#ifndef SPORTBIKERIDE_H_
#define SPORTBIKERIDE_H_

#include <sefl_quest_core.h>
#include <sys/_stdint.h>

class PCA9685;

//#include <Servo.h>

class DFRobotDFPlayerMini;

namespace SEFL
{

	class Sportbikeride : public Quest_Basic_Client
	{

		uint8_t sportbikeRideThrottle, countTurn, turnSignalsFlag, throttleFlag;
		uint8_t sportbikeRideLight, dashboardLight;
		uint8_t sportbikeServo, sportbikePowerServo;
		uint64_t timer, timerForTurnSignals;
		uint8_t sportbikeLeftRelay, sportbikeRightRelay, sportbikePowerRelay, sportbikeRightRelayFront, sportbikeLeftRelayFront, tachometerPower;

		PCA9685 *pwmController;
		DFRobotDFPlayerMini *player;

		//		Servo bikeServo;

	public:
		Sportbikeride(MQTTClient &mqtt, const char *name = DEFAULT_DEVICE_NAME,
					  uint8_t reset_status = 1, const char *placement = DEFAULT_PLACEMENT,
					  const char *in_topic = DEFAUT_IN_TOPIC, const char *out_topic = DEFAUT_OUT_TOPIC, SEFL::Language language = UKR);
		virtual ~Sportbikeride();
		virtual void onActive() override;
		virtual void onFinished() override;
		virtual void onManualFinished() override;
		virtual void onDefault() override;
		DFRobotDFPlayerMini *getPlayer();
		void setPlayer(DFRobotDFPlayerMini *player);
		uint8_t getSportbikeRideLight();
		void setSportbikeRideLight(uint8_t sportbikeRideLight);
		uint8_t getSportbikeRideThrottle();
		void setSportbikeRideThrottle(uint8_t sportbikeRideThrottle);
		uint8_t getSportbikeServo();
		void setSportbikeServo(uint8_t sportbikeServo);
		//	Servo getBikeServo() ;
		//	void setBikeServo(Servo bikeServo);
		uint8_t getSportbikeLeftRelay();
		void setSportbikeLeftRelay(uint8_t sportbikeLeftRelay);
		uint8_t getSportbikeRightRelay();
		void setSportbikeRightRelay(uint8_t sportbikeRightRelay);
		PCA9685 *&getPwmController();
		void setPwmController(PCA9685 *&pwmController);
		uint8_t getDashboardLight();
		void setDashboardLight(uint8_t dashboardLight);
		uint8_t getSportbikeLeftRelayFront();
		void setSportbikeLeftRelayFront(uint8_t sportbikeLeftRelayFront);
		uint8_t getSportbikePowerRelay();
		void setSportbikePowerRelay(uint8_t sportbikePowerRelay);
		uint8_t getSportbikeRightRelayFront();
		void setSportbikeRightRelayFront(uint8_t sportbikeRightRelayFront);
		uint8_t getTachometerPower();
		void setTachometerPower(uint8_t tachometerPower);
		uint8_t getSportbikePowerServo();
		void setSportbikePowerServo(uint8_t sportbikePowerServo);
		uint8_t getCountTurn();
		void setCountTurn(uint8_t countTurn);
		uint64_t getTimer();
		void setTimer(uint64_t timer);
		uint64_t getTimerForTurnSignals();
		void setTimerForTurnSignals(uint64_t timerForTurnSignals);
		uint8_t getTurnSignalsFlag();
		void setTurnSignalsFlag(uint8_t turnSignalsFlag);
		uint8_t getThrottleFlag();
		void setThrottleFlag(uint8_t throttleFlag);
	};

} /* namespace SEFL */

#endif /* SPORTBIKERIDE_H_ */

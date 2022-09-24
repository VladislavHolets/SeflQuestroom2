/*
 * elevatorin.h
 *
 *  Created on: Jan 31, 2022
 *      Author: piatk
 */

#ifndef ELEVATORIN_H_
#define ELEVATORIN_H_

#include <constants.h>
#include <quest_clients/questbasicclient.h>
#include <sys/_stdint.h>

namespace SEFL
{
	class Magnet;
} /* namespace SEFL */

class DFRobotDFPlayerMini;

namespace SEFL
{

	class Elevatorin : public Quest_Basic_Client
	{
		uint8_t elevatorInSwith, elevatorOutSwith;
		uint8_t elevatorInButton, elevatorInLed;
		DFRobotDFPlayerMini *player;
		Magnet *out_magnet, *in_magnet;
		uint64_t timer;

	public:
		Elevatorin(MQTTClient &mqtt, const char *name = DEFAULT_DEVICE_NAME,
				   uint8_t reset_status = 1, const char *placement = DEFAULT_PLACEMENT,
				   const char *in_topic = DEFAUT_IN_TOPIC, const char *out_topic = DEFAUT_OUT_TOPIC, SEFL::Language language = UKR);
		virtual ~Elevatorin();
		virtual void onActive() override;
		virtual void onFinished() override;
		virtual void onManualFinished() override;
		virtual void onDefault() override;
		void setPlayer(DFRobotDFPlayerMini *player);
		uint8_t getElevatorInButton();
		void setElevatorInButton(uint8_t elevatorInButton);
		uint8_t getElevatorInLed();
		void setElevatorInLed(uint8_t elevatorInLed);
		uint8_t getElevatorInSwith();
		void setElevatorInSwith(uint8_t elevatorInSwith);
		uint8_t getElevatorOutSwith();
		void setElevatorOutSwith(uint8_t elevatorOutSwith);

		Magnet *getInMagnet();
		void setInMagnet(Magnet *inMagnet);
		Magnet *getOutMagnet();
		void setOutMagnet(Magnet *outMagnet);
		uint64_t getTimer();
		void setTimer(uint64_t timer);
	};

} /* namespace SEFL */

#endif /* ELEVATORIN_H_ */

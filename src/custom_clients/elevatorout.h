/*
 * elevatorout.h
 *
 *  Created on: Jan 31, 2022
 *      Author: piatk
 */

#ifndef ELEVATOROUT_H_
#define ELEVATOROUT_H_

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

	class Elevatorout : public Quest_Basic_Client
	{

		uint8_t elevatorInSwith;
		uint8_t elevatorOutSwith;
		uint8_t flag;
		DFRobotDFPlayerMini *player;
		Magnet *out_magnet, *in_magnet;

	public:
		Elevatorout(MQTTClient &mqtt, const char *name = DEFAULT_DEVICE_NAME,
					uint8_t reset_status = 1, const char *placement = DEFAULT_PLACEMENT,
					const char *in_topic = DEFAUT_IN_TOPIC, const char *out_topic = DEFAUT_OUT_TOPIC, SEFL::Language language = UKR);
		virtual ~Elevatorout();
		virtual void onActive() override;
		virtual void onFinished() override;
		virtual void onManualFinished() override;
		virtual void onDefault() override;
		uint8_t getElevatorInSwith() const;
		void setElevatorInSwith(uint8_t elevatorInSwith);
		uint8_t getElevatorOutSwith() const;
		void setElevatorOutSwith(uint8_t elevatorOutSwith);
		DFRobotDFPlayerMini *getPlayer() const;
		void setPlayer(DFRobotDFPlayerMini *player);
		uint8_t getFlag() const;
		void setFlag(uint8_t flag);

		Magnet *getOutMagnet();
		void setOutMagnet(Magnet *outMagnet);
		Magnet *getInMagnet();
		void setInMagnet(Magnet *inMagnet);
	};

} /* namespace SEFL */

#endif /* ELEVATOROUT_H_ */

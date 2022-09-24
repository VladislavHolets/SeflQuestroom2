/*
 * labyrinth.h
 *
 *  Created on: Jan 31, 2022
 *      Author: piatk
 */

#ifndef LABYRINTH_H_
#define LABYRINTH_H_

#include <constants.h>
#include <quest_clients/questbasicclient.h>
#include <sys/_stdint.h>

namespace SEFL
{
	class Magnet;
} /* namespace SEFL */

class DFRobotDFPlayerMini;
class Adafruit_NeoPixel;

namespace SEFL
{

	class Labyrinth : public Quest_Basic_Client
	{
		uint8_t labyrinthUpButton, labyrinthDownButton, labyrinthLeftButton, labyrinthRightButton;
		uint8_t labyrinthAddressLed;
		uint8_t elevatorInSwith;
		uint8_t elevatorOutSwith;
		Magnet *out_magnet, *in_magnet;

		bool flag_finish;
		uint64_t timer;

		DFRobotDFPlayerMini *player;
		//	Adafruit_NeoPixel *pixels_;
		//	Adafruit_NeoPixel labyrinth_pixels(36, labyrinthAddressLed, NEO_GRB + NEO_KHZ800);
		//	labyrinth_pixels.begin();
		uint8_t way;
		bool holdFlag;

	public:
		Labyrinth(MQTTClient &mqtt, const char *name = DEFAULT_DEVICE_NAME,
				  uint8_t reset_status = 1, const char *placement = DEFAULT_PLACEMENT,
				  const char *in_topic = DEFAUT_IN_TOPIC, const char *out_topic = DEFAUT_OUT_TOPIC, SEFL::Language language = UKR);
		virtual ~Labyrinth();
		virtual void onActive() override;
		virtual void onFinished() override;
		virtual void onManualFinished() override;
		virtual void onDefault() override;
		uint8_t getLabyrinthAddressLed() const;
		void setLabyrinthAddressLed(uint8_t labyrinthAddressLed);
		uint8_t getLabyrinthLeftButton() const;
		void setLabyrinthLeftButton(uint8_t labyrinthLeftButton);
		uint8_t getLabyrinthRightButton() const;
		void setLabyrinthRightButton(uint8_t labyrinthRightButton);
		DFRobotDFPlayerMini *getPlayer() const;
		void setPlayer(DFRobotDFPlayerMini *player);
		uint8_t getElevatorInSwith() const;
		void setElevatorInSwith(uint8_t elevatorInSwith);
		uint8_t getElevatorOutSwith() const;
		void setElevatorOutSwith(uint8_t elevatorOutSwith);
		uint8_t getLabyrinthDownButton() const;
		void setLabyrinthDownButton(uint8_t labyrinthDownButton);
		uint8_t getLabyrinthUpButton() const;
		void setLabyrinthUpButton(uint8_t labyrinthUpButton);
		bool isFlagFinish() const;
		void setFlagFinish(bool flagFinish);
		uint64_t getTimer() const;
		void setTimer(uint64_t timer);
		Magnet *getInMagnet();
		void setInMagnet(Magnet *inMagnet);
		Magnet *getOutMagnet();
		void setOutMagnet(Magnet *outMagnet);
	};

} /* namespace SEFL */

#endif /* LABYRINTH_H_ */

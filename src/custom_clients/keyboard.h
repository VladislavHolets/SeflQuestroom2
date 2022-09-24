/*
 * keyboard.h
 *
 *  Created on: Jan 31, 2022
 *      Author: piatk
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <constants.h>
#include <quest_clients/questbasicclient.h>
#include <sys/_stdint.h>

class DFRobotDFPlayerMini;

namespace SEFL
{

	class Keyboard : public Quest_Basic_Client
	{
		uint8_t keyboardPin;
		uint8_t keyboardMagnet;
		uint8_t fireplaceSmokeRelay;
		DFRobotDFPlayerMini *player;

	public:
		Keyboard(MQTTClient &mqtt, const char *name = DEFAULT_DEVICE_NAME,
				 uint8_t reset_status = 1, const char *placement = DEFAULT_PLACEMENT,
				 const char *in_topic = DEFAUT_IN_TOPIC, const char *out_topic = DEFAUT_OUT_TOPIC, SEFL::Language language = UKR);
		virtual ~Keyboard();
		virtual void onActive() override;
		virtual void onFinished() override;
		virtual void onManualFinished() override;
		virtual void onDefault() override;
		uint8_t getKeyboardMagnet();
		void setKeyboardMagnet(uint8_t keyboardMagnet);
		uint8_t getKeyboardPin();
		void setKeyboardPin(uint8_t keyboardPin);
		DFRobotDFPlayerMini *getPlayer();
		void setPlayer(DFRobotDFPlayerMini *player);
		uint8_t getFireplaceSmokeRelay();
		void setFireplaceSmokeRelay(uint8_t fireplaceSmokeRelay);
	};

} /* namespace SEFL */

#endif /* KEYBOARD_H_ */

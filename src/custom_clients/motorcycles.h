/*
 * motorcycles.h
 *
 *  Created on: Jan 31, 2022
 *      Author: piatk
 */

#ifndef MOTORCYCLES_H_
#define MOTORCYCLES_H_

#include <constants.h>
#include <quest_clients/questbasicclient.h>
#include <sys/_stdint.h>

class Adafruit_NeoPixel;

class DFRobotDFPlayerMini;

namespace SEFL
{

	class Motorcycles : public Quest_Basic_Client
	{
		uint8_t pot_l, pot_c, pot_r;
		uint8_t arduino_in;
		uint8_t arduino_power;
		uint8_t motorcyclesAddressLed;
		DFRobotDFPlayerMini *player;
		Adafruit_NeoPixel *pixels_;

	public:
		Motorcycles(MQTTClient &mqtt, const char *name = DEFAULT_DEVICE_NAME,
					uint8_t reset_status = 1, const char *placement = DEFAULT_PLACEMENT,
					const char *in_topic = DEFAUT_IN_TOPIC, const char *out_topic = DEFAUT_OUT_TOPIC, SEFL::Language language = UKR);
		virtual ~Motorcycles();
		virtual void onActive() override;
		virtual void onFinished() override;
		virtual void onManualFinished() override;
		virtual void onDefault() override;
		uint8_t getMotorcyclesAddressLed();
		void setMotorcyclesAddressLed(uint8_t motorcyclesAddressLed);
		DFRobotDFPlayerMini *getPlayer();
		void setPlayer(DFRobotDFPlayerMini *player);
		uint8_t getPotC();
		void setPotC(uint8_t potC);
		uint8_t getPotL();
		void setPotL(uint8_t potL);
		uint8_t getPotR();
		void setPotR(uint8_t potR);
		Adafruit_NeoPixel *&getPixels();
		void setPixels(Adafruit_NeoPixel *&pixels);
		uint8_t getArduinoIn();
		void setArduinoIn(uint8_t arduinoIn);
		uint8_t getArduinoPower();
		void setArduinoPower(uint8_t arduinoPower);
	};

} /* namespace SEFL */

#endif /* MOTORCYCLES_H_ */

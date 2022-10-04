/*
 * tornado.h
 *
 *  Created on: Jan 27, 2022
 *      Author: piatk
 */

#ifndef TORNADO_H_
#define TORNADO_H_

#include <sefl_quest_core.h>
#include <sys/_stdint.h>

class DFRobotDFPlayerMini;

namespace SEFL
{

	class Tornado : public Quest_Basic_Client
	{
		uint8_t tornado_magnet, motor_left, motor_centre, motor_right;
		uint8_t rgb_light_tornado_r, rgb_light_tornado_g, rgb_light_tornado_b;
		uint8_t rgb_light_underwather_r, rgb_light_underwather_g, rgb_light_underwather_b;
		uint8_t pot_lt, pot_lc, pot_lb;
		DFRobotDFPlayerMini *player;
		uint64_t timing;

	public:
		Tornado(MQTTClient &mqtt, const char *name = DEFAULT_DEVICE_NAME,
				uint8_t reset_status = 1, const char *placement = DEFAULT_PLACEMENT,
				const char *in_topic = DEFAUT_IN_TOPIC, const char *out_topic = DEFAUT_OUT_TOPIC, SEFL::Language language = UKR);
		virtual ~Tornado();
		virtual void onActive() override;
		virtual void onFinished() override;
		virtual void onManualFinished() override;
		virtual void onDefault() override;

		uint8_t getMotorCentre();
		void setMotorCentre(uint8_t motorCentre);
		uint8_t getMotorLeft();
		void setMotorLeft(uint8_t motorLeft);
		void setPlayer(DFRobotDFPlayerMini *player);
		uint8_t getPotLb();
		void setPotLb(uint8_t potLb);
		uint8_t getPotLc();
		void setPotLc(uint8_t potLc);
		uint8_t getPotLt();
		void setPotLt(uint8_t potLt);
		uint8_t getRgbLightTornadoB();
		void setRgbLightTornadoB(uint8_t rgbLightTornadoB);
		uint8_t getRgbLightTornadoG();
		void setRgbLightTornadoG(uint8_t rgbLightTornadoG);
		uint8_t getRgbLightTornadoR();
		void setRgbLightTornadoR(uint8_t rgbLightTornadoR);
		uint8_t getRgbLightUnderwatherB();
		void setRgbLightUnderwatherB(uint8_t rgbLightUnderwatherB);
		uint8_t getRgbLightUnderwatherG();
		void setRgbLightUnderwatherG(uint8_t rgbLightUnderwatherG);
		uint8_t getRgbLightUnderwatherR();
		void setRgbLightUnderwatherR(uint8_t rgbLightUnderwatherR);
		uint8_t getTornadoMagnet();
		void setTornadoMagnet(uint8_t tornadoMagnet);
		uint8_t getMotorRight();
		void setMotorRight(uint8_t motorRight);
		uint64_t getTiming();
		void setTiming(uint64_t timing);
	};

} /* namespace SEFL */

#endif /* TORNADO_H_ */

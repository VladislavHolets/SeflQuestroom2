/*
 * underwater.h
 *
 *  Created on: Jan 26, 2022
 *      Author: piatk
 */

#ifndef UNDERWATER_H_
#define UNDERWATER_H_

#include <constants.h>
#include <quest_clients/questbasicclient.h>
#include <sys/_stdint.h>

class DFRobotDFPlayerMini;

namespace SEFL
{

	class Underwater : public Quest_Basic_Client
	{
		uint8_t encoder_left_a, encoder_right_a;
		uint8_t rgb_light_underwather_r, rgb_light_underwather_g, rgb_light_underwather_b;
		uint8_t button_volume, level_water, pump, flag_final;
		uint64_t timing, timing_final;
		bool inverted, encoder_swith_left, encoder_swith_right, encoder_flag_right, encoder_flag_left;
		DFRobotDFPlayerMini *player;

	public:
		Underwater(MQTTClient &mqtt, const char *name = DEFAULT_DEVICE_NAME,
				   uint8_t reset_status = 1, const char *placement = DEFAULT_PLACEMENT,
				   const char *in_topic = DEFAUT_IN_TOPIC, const char *out_topic = DEFAUT_OUT_TOPIC, SEFL::Language language = UKR);
		virtual ~Underwater();
		virtual void onActive() override;
		virtual void onFinished() override;
		virtual void onManualFinished() override;
		virtual void onDefault() override;
		uint8_t getButtonVolume() const;
		void setButtonVolume(uint8_t buttonVolume);
		uint8_t getEncoderLeftA() const;
		void setEncoderLeftA(uint8_t encoderLeftA);
		uint8_t getEncoderRightA() const;
		void setEncoderRightA(uint8_t encoderRightA);
		bool isInverted() const;
		void setInverted(bool inverted);
		uint8_t getLevelWater() const;
		void setLevelWater(uint8_t levelWater);
		uint8_t getPump() const;
		void setPump(uint8_t pump);
		uint8_t getRgbLightUnderwatherB() const;
		void setRgbLightUnderwatherB(uint8_t rgbLightUnderwatherB);
		uint8_t getRgbLightUnderwatherG() const;
		void setRgbLightUnderwatherG(uint8_t rgbLightUnderwatherG);
		uint8_t getRgbLightUnderwatherR() const;
		void setRgbLightUnderwatherR(uint8_t rgbLightUnderwatherR);

		void setPlayer(DFRobotDFPlayerMini *player);
		uint64_t getTiming() const;
		void setTiming(uint64_t timing);
		bool isEncoderSwithLeft() const;
		void setEncoderSwithLeft(bool encoderSwithLeft);
		bool isEncoderSwithRight() const;
		void setEncoderSwithRight(bool encoderSwithRight);
		bool isEncoderFlagLeft() const;
		void setEncoderFlagLeft(bool encoderFlagLeft);
		bool isEncoderFlagRight() const;
		void setEncoderFlagRight(bool encoderFlagRight);
		uint8_t getFlagFinal() const;
		void setFlagFinal(uint8_t flagFinal);
		uint64_t getTimingFinal() const;
		void setTimingFinal(uint64_t timingFinal);
	};

} /* namespace SEFL */

#endif /* UNDERWATER_H_ */

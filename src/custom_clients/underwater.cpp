/*
 * underwater.cpp
 *
 *  Created on: Jan 26, 2022
 *      Author: piatk
 */

#include <DFRobotDFPlayerMini.h>
#include <Printable.h>
#include <utils/logger.h>
#include <peripherals/multpinsextender.h>
#include <peripherals/pwmpinsextender.h>
#include <custom_clients/underwater.h>
#include <wiring_constants.h>
#include <wiring_digital.h>
#include <WString.h>

namespace SEFL
{

	Underwater::Underwater(MQTTClient &mqtt, const char *name, uint8_t reset_status,
						   const char *placement, const char *in_topic, const char *out_topic,
						   SEFL::Language language) : Quest_Basic_Client(mqtt, name, reset_status, placement, in_topic,
																		 out_topic, language)
	{
		encoder_left_a = 0;
		encoder_right_a = 2;
		rgb_light_underwather_r = 0;
		rgb_light_underwather_g = 1;
		rgb_light_underwather_b = 2;
		button_volume = 4;
		level_water = 5;
		pump = 3;
		inverted = 1;
		player = nullptr;
		timing = 0;
		encoder_swith_left = true;
		encoder_swith_right = true;
		encoder_flag_right = false;
		encoder_flag_left = false;

		flag_final = 0;
		timing_final = 0;
	}

	Underwater::~Underwater()
	{
		// TODO Auto-generated destructor stub
	}

	void Underwater::onActive()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("Underwater", "on Active ");
			flag_final = 0;
			if (player != nullptr)
			{
				player->play(1);
				SEFL::Logger::verbose("Underwater", "0001.mp3");
				//	player = nullptr;
			}
			Mext.digitalRead(button_volume);
			pinMode(Mext.getCi(), INPUT_PULLUP);
		}

		//		SEFL::Logger::verbose("Underwater","button_volume ");
		//		SEFL::Logger::verbose("Underwater",Mext.digitalRead(button_volume));

		// Pext.analogWrite(rgb_light_underwather_r, 500);
		// Pext.analogWrite(rgb_light_underwather_g, 1500);
		Pext.analogWrite(rgb_light_underwather_b, 4096);
		if (player != nullptr && flag_final == 0)
		{
			if (Mext.digitalRead(button_volume))
			{
				player->volume(25);
			}
			else
			{
				player->volume(0);
			}
		}

		//	if (encoder_swith_left != encoder_left_a && encoder_swith_right != encoder_right_a) {
		//		Pext.digitalWrite(pump, HIGH);
		//		encoder_swith_left = !encoder_swith_left;
		//		encoder_swith_right = !encoder_swith_right;
		//		timing = millis();
		//		SEFL::Logger::verbose("Underwater",Mext.digitalRead(encoder_left_a));
		//			SEFL::Logger::verbose("Underwater    ",Mext.digitalRead(encoder_right_a));
		//	}

		//	SEFL::Logger::verbose("Underwatl", encoder_flag_left);
		//	SEFL::Logger::verbose("Underwater", encoder_flag_right);
		//	SEFL::Logger::verbose("Underwater right", Mext.digitalRead(encoder_right_a));

		if (encoder_swith_left != Mext.digitalRead(encoder_left_a))
		{
			encoder_swith_left = !encoder_swith_left;
			SEFL::Logger::verbose("Underwater left",
								  Mext.digitalRead(encoder_left_a));
			encoder_flag_left = true;
		}

		if (encoder_swith_right != Mext.digitalRead(encoder_right_a))
		{
			encoder_swith_right = !encoder_swith_right;
			SEFL::Logger::verbose("Underwater right",
								  Mext.digitalRead(encoder_right_a));
			encoder_flag_right = true;
		}

		if (encoder_flag_left && encoder_flag_right && flag_final == 0)
		{
			Pext.digitalWrite(pump, HIGH);
			timing = millis();
			encoder_flag_left = false;
			encoder_flag_right = false;
		}

		if ((millis() - timing > 3000))
		{
			Pext.digitalWrite(pump, LOW);
			encoder_flag_left = false;
			encoder_flag_right = false;
		}

		//	if (Mext.digitalRead(encoderleft_b)) {
		//			SEFL::Logger::verbose("Underwater","encoder_left_b");
		//		}
		//	if (Mext.digitalRead(encoder_right_a)) {
		//			SEFL::Logger::verbose("Underwater","encoder_right_a");
		//		}
		//	if (Mext.digitalRead(encoder_right_b)) {
		//			SEFL::Logger::verbose("Underwater","encoder_right_b");
		//		}

		//	if (!Mext.digitalRead(encoder_left_a) && !Mext.digitalRead(encoderleft_b) && !Mext.digitalRead(encoder_right_a) && !Mext.digitalRead(encoder_right_b)) {
		//		Pext.digitalWrite(pump, HIGH);
		//		SEFL::Logger::verbose("Underwater","pump ON");
		//
		//	}

		if (Mext.digitalRead(level_water) == HIGH)
		{
			Pext.digitalWrite(pump, LOW);
			flag_final = 2;
		}

		if (flag_final == 2)
		{

			SEFL::Logger::verbose("Underwater", "pump OFF");

			Pext.digitalWrite(pump, LOW);
			//		Pext.analogWrite(rgb_light_underwather_r, 4096);
			//		Pext.analogWrite(rgb_light_underwather_g, 4096);
			//		Pext.analogWrite(rgb_light_underwather_b, 4096);

			if (player != nullptr)
			{
				player->volume(20);
				switch (this->getLanguage())
				{
				case SEFL::Language::ENG:
				{
					player->play(2);
					SEFL::Logger::verbose("Tornado", "0003.mp3");
				}
				break;
				case SEFL::Language::RUS:
				{
					player->play(8);
					SEFL::Logger::verbose("Tornado", "0008.mp3");
				}
				break;
				case SEFL::Language::UKR:
				{
					player->play(9);
					SEFL::Logger::verbose("Tornado", "0009.mp3");
				}
				break;
				}
				flag_final = 1;
				timing_final = millis();

				// SEFL::Logger::verbose("Tornado", "0003.mp3");
			}
		}
		if (flag_final == 1 && millis() - timing_final > 17000)
		{

			this->setStatus(BasicClientStatuses::FINISHED_STATUS);
		}
	}

	void Underwater::onFinished()
	{

		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("Underwater", "on Finished ");

			Pext.digitalWrite(pump, LOW);
			Pext.analogWrite(rgb_light_underwather_r, 4096);
			Pext.analogWrite(rgb_light_underwather_g, 4096);
			Pext.analogWrite(rgb_light_underwather_b, 4096);
		}
	}
	//}

	void Underwater::onManualFinished()
	{

		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("Underwater", "on Manual Finished ");
			//
			//		if (player != nullptr) {
			//			player->volume(20);
			//			player->play(2);
			//			SEFL::Logger::verbose("Underwater", "0002.mp3");
			//		}
			Pext.digitalWrite(pump, LOW);
			Pext.analogWrite(rgb_light_underwather_r, 4096);
			Pext.analogWrite(rgb_light_underwather_g, 4096);
			Pext.analogWrite(rgb_light_underwather_b, 4096);
		}
	}

	void Underwater::onDefault()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("Underwater", "on Default");

			if (player != nullptr)
			{
				player->stop();
			}

			Pext.digitalWrite(pump, LOW);
			Pext.analogWrite(rgb_light_underwather_r, 0);
			Pext.analogWrite(rgb_light_underwather_g, 0);
			Pext.analogWrite(rgb_light_underwather_b, 0);
		}
	}

	uint8_t Underwater::getButtonVolume() const
	{
		return button_volume;
	}

	void Underwater::setButtonVolume(uint8_t buttonVolume)
	{
		button_volume = buttonVolume;
	}

	uint8_t Underwater::getEncoderLeftA() const
	{
		return encoder_left_a;
	}

	void Underwater::setEncoderLeftA(uint8_t encoderLeftA)
	{
		encoder_left_a = encoderLeftA;
	}

	uint8_t Underwater::getEncoderRightA() const
	{
		return encoder_right_a;
	}

	void Underwater::setEncoderRightA(uint8_t encoderRightA)
	{
		encoder_right_a = encoderRightA;
	}

	bool Underwater::isInverted() const
	{
		return inverted;
	}

	void Underwater::setInverted(bool inverted)
	{
		this->inverted = inverted;
	}

	uint8_t Underwater::getLevelWater() const
	{
		return level_water;
	}

	void Underwater::setLevelWater(uint8_t levelWater)
	{
		level_water = levelWater;
	}

	uint8_t Underwater::getPump() const
	{
		return pump;
	}

	void Underwater::setPump(uint8_t pump)
	{
		this->pump = pump;
	}

	uint8_t Underwater::getRgbLightUnderwatherB() const
	{
		return rgb_light_underwather_b;
	}

	void Underwater::setRgbLightUnderwatherB(uint8_t rgbLightUnderwatherB)
	{
		rgb_light_underwather_b = rgbLightUnderwatherB;
	}

	uint8_t Underwater::getRgbLightUnderwatherG() const
	{
		return rgb_light_underwather_g;
	}

	void Underwater::setRgbLightUnderwatherG(uint8_t rgbLightUnderwatherG)
	{
		rgb_light_underwather_g = rgbLightUnderwatherG;
	}

	uint8_t Underwater::getRgbLightUnderwatherR() const
	{
		return rgb_light_underwather_r;
	}

	uint64_t Underwater::getTiming() const
	{
		return timing;
	}

	void Underwater::setTiming(uint64_t timing)
	{
		this->timing = timing;
	}

	bool Underwater::isEncoderSwithLeft() const
	{
		return encoder_swith_left;
	}

	void Underwater::setEncoderSwithLeft(bool encoderSwithLeft)
	{
		encoder_swith_left = encoderSwithLeft;
	}

	bool Underwater::isEncoderSwithRight() const
	{
		return encoder_swith_right;
	}

	bool Underwater::isEncoderFlagLeft() const
	{
		return encoder_flag_left;
	}

	void Underwater::setEncoderFlagLeft(bool encoderFlagLeft)
	{
		encoder_flag_left = encoderFlagLeft;
	}

	bool Underwater::isEncoderFlagRight() const
	{
		return encoder_flag_right;
	}

	uint8_t Underwater::getFlagFinal() const
	{
		return flag_final;
	}

	void Underwater::setFlagFinal(uint8_t flagFinal)
	{
		flag_final = flagFinal;
	}

	uint64_t Underwater::getTimingFinal() const
	{
		return timing_final;
	}

	void Underwater::setTimingFinal(uint64_t timingFinal)
	{
		timing_final = timingFinal;
	}

	void Underwater::setEncoderFlagRight(bool encoderFlagRight)
	{
		encoder_flag_right = encoderFlagRight;
	}

	void Underwater::setEncoderSwithRight(bool encoderSwithRight)
	{
		encoder_swith_right = encoderSwithRight;
	}

	void Underwater::setPlayer(DFRobotDFPlayerMini *player)
	{
		this->player = player;
	}

	void Underwater::setRgbLightUnderwatherR(uint8_t rgbLightUnderwatherR)
	{
		rgb_light_underwather_r = rgbLightUnderwatherR;
	}

} /* namespace SEFL */

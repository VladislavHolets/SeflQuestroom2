/*
 * tornado.cpp
 *
 *  Created on: Jan 27, 2022
 *      Author: piatk
 */

#include <DFRobotDFPlayerMini.h>
#include <utils/logger.h>
#include <peripherals/multpinsextender.h>
#include <peripherals/pwmpinsextender.h>
#include <custom_clients/tornado.h>
#include <wiring_constants.h>
#include <wiring_digital.h>
#include <wiring_time.h>
#include <WMath.h>

uint64_t timer;
uint64_t timer_for_final_voise;
uint16_t flag_final;
namespace SEFL
{

	Tornado::Tornado(MQTTClient &mqtt, const char *name, uint8_t reset_status,
					 const char *placement, const char *in_topic, const char *out_topic,
					 SEFL::Language language) : Quest_Basic_Client(mqtt, name, reset_status, placement, in_topic,
																   out_topic, language)
	{
		tornado_magnet = 0;
		motor_left = 1;
		motor_centre = 2;
		motor_right = 3;
		rgb_light_tornado_r = 4;
		rgb_light_tornado_g = 5;
		rgb_light_tornado_b = 6;
		rgb_light_underwather_r = 7;
		rgb_light_underwather_g = 8;
		rgb_light_underwather_b = 9;
		pot_lt = 0;
		pot_lc = 1;
		pot_lb = 2;
		player = nullptr;
		timing = 0;
	}

	Tornado::~Tornado()
	{
		// TODO Auto-generated destructor stub
	}

	void Tornado::onActive()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();

			Pext.analogWrite(rgb_light_tornado_b, 4096);
			Pext.analogWrite(rgb_light_tornado_g, 2096);
			Pext.analogWrite(rgb_light_tornado_r, 2096);

			Pext.analogWrite(motor_left, 0);
			Pext.analogWrite(motor_centre, 0);
			Pext.analogWrite(motor_right, 0);
			timing = 0;
			flag_final = 0;

			Mext.digitalRead(pot_lt);
			pinMode(Mext.getCi(), INPUT_PULLUP);
			timer = millis();
		}

		bool flag_flask_left = false, flag_flask_centre = false, flag_flask_right = false;
		Pext.digitalWrite(tornado_magnet, HIGH);

		SEFL::Logger::verbose("pot_lt", Mext.digitalRead(pot_lt));				   // and rc
		SEFL::Logger::verbose("pot_lc        ", Mext.digitalRead(pot_lc));		   // and rb
		SEFL::Logger::verbose("pot_lb                ", Mext.digitalRead(pot_lb)); // and rt

		if (Mext.digitalRead(pot_lt) == LOW)
		{
			SEFL::Logger::verbose("motor_left ", "ON");
			Pext.analogWrite(motor_left, 4096);
			flag_flask_left = true;
		}

		if (Mext.digitalRead(pot_lc) == LOW)
		{
			SEFL::Logger::verbose("motor_centre ", "ON");
			Pext.analogWrite(motor_centre, 4096);
			flag_flask_centre = true;
		}

		if (Mext.digitalRead(pot_lb) == LOW)
		{
			SEFL::Logger::verbose("motor_right ", "ON");
			Pext.analogWrite(motor_right, 4096);
			flag_flask_right = true;
		}
		if (millis() - timing > 2000)
		{

			if (!flag_flask_left)
			{

				Pext.analogWrite(motor_left, 0);
				timer = millis();
				SEFL::Logger::verbose("RESET ",
									  "-------------------------------------------------------------11111");
			}
			if (!flag_flask_centre)
			{
				Pext.analogWrite(motor_centre, 0);
				timer = millis();
				SEFL::Logger::verbose("RESET ",
									  "-------------------------------------------------------------22222");
			}
			if (!flag_flask_right)
			{
				Pext.analogWrite(motor_right, 0);
				timer = millis();
				SEFL::Logger::verbose("RESET ",
									  "-------------------------------------------------------------33333");
			}
		}

		if (flag_flask_left && flag_flask_centre && flag_flask_right && millis() - timer > 300)
		{
			SEFL::Logger::verbose("FINISH ",
								  "-------------------------------------------------------------");
			if (timing == 0)
				timing = millis();
			Pext.analogWrite(rgb_light_underwather_b, random(4000));
			Pext.analogWrite(rgb_light_tornado_b, random(4096));
			Pext.analogWrite(rgb_light_tornado_g, random(4096));
			Pext.analogWrite(rgb_light_tornado_r, random(4096));

			if (millis() - timing > 12000)
			{

				//	delay(2000);
				//			Pext.analogWrite(rgb_light_underwather_r, 4096);
				//			Pext.analogWrite(rgb_light_underwather_g, 4096);
				//			Pext.analogWrite(rgb_light_underwather_b, 4096);
				//
				//			Pext.analogWrite(motor_right, 0);
				//			Pext.analogWrite(motor_left, 0);
				//			Pext.analogWrite(motor_centre, 0);

				if (flag_final != 1)
				{
					//				if (player != nullptr) {
					//					player->play(3);
					//					SEFL::Logger::verbose("Tornado", "0003.mp3");
					//				}
					flag_final = 1;
					timer_for_final_voise = millis();
				}
			}

			if (millis() - timer_for_final_voise > 7000 && flag_final == 1)
			{
				this->setStatus(BasicClientStatuses::FINISHED_STATUS);
			}
		}
	}

	void Tornado::onFinished()
	{

		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("Tornado", "on Finished ");

			if (player != nullptr)
			{
				player->volume(17);
				switch (this->getLanguage())
				{
				case SEFL::Language::ENG:
				{
					player->play(3);
					SEFL::Logger::verbose("Tornado", "0003.mp3");
				}
				break;
				case SEFL::Language::RUS:
				{
					player->play(6);
					SEFL::Logger::verbose("Tornado", "0006.mp3");
				}
				break;
				case SEFL::Language::UKR:
				{
					player->play(7);
					SEFL::Logger::verbose("Tornado", "0007.mp3");
				}
				break;
				}

				// SEFL::Logger::verbose("Tornado", "0003.mp3");
			}

			Pext.analogWrite(rgb_light_underwather_b, 4096);
			Pext.analogWrite(rgb_light_tornado_b, 0);
			Pext.analogWrite(rgb_light_tornado_g, 0);
			Pext.analogWrite(rgb_light_tornado_r, 0);

			Pext.analogWrite(motor_right, 0);
			Pext.analogWrite(motor_left, 0);
			Pext.analogWrite(motor_centre, 0);
		}
	}

	void Tornado::onManualFinished()
	{

		if (isChangedStatus())
		{
			unsetChangedStatus();
			timer = millis();
			flag_final = 0;

			this->reportStatus();
			SEFL::Logger::verbose("Tornado", "onManualFinished");
		}
		//		if (player != nullptr) {
		//			player->volume(20);
		//			player->play(3);
		//			SEFL::Logger::verbose("Tornado", "0003.mp3");
		//		}
		//
		//		Pext.analogWrite(rgb_light_underwather_b, 4096);
		//		Pext.analogWrite(rgb_light_tornado_b, 0);
		//		Pext.analogWrite(rgb_light_tornado_g, 0);
		//		Pext.analogWrite(rgb_light_tornado_r, 0);
		//
		//		Pext.analogWrite(motor_right, 0);
		//		Pext.analogWrite(motor_left, 0);
		//		Pext.analogWrite(motor_centre, 0);

		if (millis() - timer > 100 && flag_final == 0)
		{
			Pext.analogWrite(motor_centre, 4096);
		}
		if (millis() - timer > 1000 && flag_final == 0)
		{
			Pext.analogWrite(motor_left, 4096);
		}
		if (millis() - timer > 2000 && flag_final == 0)
		{
			Pext.analogWrite(motor_right, 4096);

			Pext.analogWrite(rgb_light_tornado_b, random(4096));
			Pext.analogWrite(rgb_light_tornado_g, random(4096));
			Pext.analogWrite(rgb_light_tornado_r, random(4096));
		}
		if (millis() - timer > 12000 && flag_final == 0)
		{
			// onFinished();
			if (player != nullptr)
			{
				player->volume(17);
				switch (this->getLanguage())
				{
				case SEFL::Language::ENG:
				{
					player->play(3);
					SEFL::Logger::verbose("Tornado", "0003.mp3");
				}
				break;
				case SEFL::Language::RUS:
				{
					player->play(6);
					SEFL::Logger::verbose("Tornado", "0006.mp3");
				}
				break;
				case SEFL::Language::UKR:
				{
					player->play(7);
					SEFL::Logger::verbose("Tornado", "0007.mp3");
				}
				break;
				}

				// SEFL::Logger::verbose("Tornado", "0003.mp3");
			}
			flag_final = 1;
		}
		if (flag_final == 1)
		{
			Pext.analogWrite(rgb_light_underwather_b, 4096);
			Pext.analogWrite(rgb_light_tornado_b, 0);
			Pext.analogWrite(rgb_light_tornado_g, 0);
			Pext.analogWrite(rgb_light_tornado_r, 0);

			Pext.analogWrite(motor_right, 0);
			Pext.analogWrite(motor_left, 0);
			Pext.analogWrite(motor_centre, 0);
		}
	}

	void Tornado::onDefault()
	{

		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("Tornado", "onDefault");

			Pext.digitalWrite(tornado_magnet, LOW);

			if (player != nullptr)
			{
				player->stop();
			}

			Pext.analogWrite(rgb_light_tornado_b, 0);
			Pext.analogWrite(rgb_light_tornado_g, 0);
			Pext.analogWrite(rgb_light_tornado_r, 0);
			Pext.analogWrite(motor_right, 0);
			Pext.analogWrite(motor_left, 0);
			Pext.analogWrite(motor_centre, 0);
		}
	}

	uint8_t Tornado::getMotorCentre() const
	{
		return motor_centre;
	}

	void Tornado::setMotorCentre(uint8_t motorCentre)
	{
		motor_centre = motorCentre;
	}

	uint8_t Tornado::getMotorLeft() const
	{
		return motor_left;
	}

	void Tornado::setMotorLeft(uint8_t motorLeft)
	{
		motor_left = motorLeft;
	}

	void Tornado::setPlayer(DFRobotDFPlayerMini *player)
	{
		this->player = player;
	}

	uint8_t Tornado::getPotLb() const
	{
		return pot_lb;
	}

	void Tornado::setPotLb(uint8_t potLb)
	{
		pot_lb = potLb;
	}

	uint8_t Tornado::getPotLc() const
	{
		return pot_lc;
	}

	void Tornado::setPotLc(uint8_t potLc)
	{
		pot_lc = potLc;
	}

	uint8_t Tornado::getPotLt() const
	{
		return pot_lt;
	}

	void Tornado::setPotLt(uint8_t potLt)
	{
		pot_lt = potLt;
	}

	uint8_t Tornado::getRgbLightTornadoB() const
	{
		return rgb_light_tornado_b;
	}

	void Tornado::setRgbLightTornadoB(uint8_t rgbLightTornadoB)
	{
		rgb_light_tornado_b = rgbLightTornadoB;
	}

	uint8_t Tornado::getRgbLightTornadoG() const
	{
		return rgb_light_tornado_g;
	}

	void Tornado::setRgbLightTornadoG(uint8_t rgbLightTornadoG)
	{
		rgb_light_tornado_g = rgbLightTornadoG;
	}

	uint8_t Tornado::getRgbLightTornadoR() const
	{
		return rgb_light_tornado_r;
	}

	void Tornado::setRgbLightTornadoR(uint8_t rgbLightTornadoR)
	{
		rgb_light_tornado_r = rgbLightTornadoR;
	}

	uint8_t Tornado::getRgbLightUnderwatherB() const
	{
		return rgb_light_underwather_b;
	}

	void Tornado::setRgbLightUnderwatherB(uint8_t rgbLightUnderwatherB)
	{
		rgb_light_underwather_b = rgbLightUnderwatherB;
	}

	uint8_t Tornado::getRgbLightUnderwatherG() const
	{
		return rgb_light_underwather_g;
	}

	void Tornado::setRgbLightUnderwatherG(uint8_t rgbLightUnderwatherG)
	{
		rgb_light_underwather_g = rgbLightUnderwatherG;
	}

	uint8_t Tornado::getRgbLightUnderwatherR() const
	{
		return rgb_light_underwather_r;
	}

	void Tornado::setRgbLightUnderwatherR(uint8_t rgbLightUnderwatherR)
	{
		rgb_light_underwather_r = rgbLightUnderwatherR;
	}

	uint8_t Tornado::getTornadoMagnet() const
	{
		return tornado_magnet;
	}

	uint8_t Tornado::getMotorRight() const
	{
		return motor_right;
	}

	uint64_t Tornado::getTiming() const
	{
		return timing;
	}

	void Tornado::setTiming(uint64_t timing)
	{
		this->timing = timing;
	}

	void Tornado::setMotorRight(uint8_t motorRight)
	{
		motor_right = motorRight;
	}

	void Tornado::setTornadoMagnet(uint8_t tornadoMagnet)
	{
		tornado_magnet = tornadoMagnet;
	}

} /* namespace SEFL */

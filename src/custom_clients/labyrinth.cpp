/*
 * labyrinth.cpp
 *
 *  Created on: Jan 31, 2022
 *      Author: piatk
 */

#include <Adafruit_NeoPixel.h>
#include <DFRobotDFPlayerMini.h>
#include "labyrinth.h"
#include "magnet.h"
#include <wiring_constants.h>
#include <wiring_digital.h>
#include <wiring_time.h>

uint8_t prevbutt = 0;
uint8_t way;

uint8_t pin_butt1 = 1;
uint8_t pin_butt2 = 2;
uint8_t pin_butt3 = 3;
uint8_t pin_butt4 = 4;
// Adafruit_NeoPixel pixels_;
class DFRobotDFPlayerMini;

namespace SEFL
{

	uint8_t labyrintLED = SEFL::uext_config.SSEL; ////;5  //  PB_2
#define NUMPIXELS 36							  //

	Adafruit_NeoPixel labyrintpixels(NUMPIXELS, labyrintLED, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 50 // Time (in milliseconds) to pause between pixels

	Labyrinth::Labyrinth(MQTTClient &mqtt, const char *name, uint8_t reset_status,
						 const char *placement, const char *in_topic, const char *out_topic,
						 SEFL::Language language) : Quest_Basic_Client(mqtt, name, reset_status, placement, in_topic,
																	   out_topic, language)
	{
		labyrinthUpButton = 0;
		labyrinthDownButton = 1;

		labyrinthLeftButton = 2;
		labyrinthRightButton = 3;
		labyrinthAddressLed = 4;
		elevatorInSwith = 5;
		elevatorOutSwith = 6;
		player = nullptr;
		way = 0;
		holdFlag = false;
		flag_finish = false;
		timer = 0;
	}

	void Labyrinth::onActive()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("Labyrinth", "onActive");

			way = 0;
			labyrintpixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

			Mext.digitalRead(labyrinthUpButton);
			pinMode(Mext.getCi(), INPUT_PULLUP);
			flag_finish = false;
			if (player != nullptr)
			{
				player->volume(25);
			}
		}

		// digitalRead(labyrinthUpButton);

		uint8_t butt;

		if (!Mext.digitalRead(labyrinthUpButton))
		{
			SEFL::Logger::verbose("Labyrinth", "labyrinthUpButton");
			butt = 1;
		}
		else if (!Mext.digitalRead(labyrinthDownButton))
		{
			SEFL::Logger::verbose("Labyrinth", "labyrinthDownButton");
			butt = 2;
		}
		else if (!Mext.digitalRead(labyrinthLeftButton))
		{
			SEFL::Logger::verbose("Labyrinth", "labyrinthLeftButton");
			butt = 3;
		}
		else if (!Mext.digitalRead(labyrinthRightButton))
		{
			SEFL::Logger::verbose("Labyrinth", "labyrinthRightButton");
			butt = 4;
		}
		else
			butt = 0;

		if (prevbutt != butt && butt != 0)
		{
			// SEFL::Logger::verbose("butt", butt);
			SEFL::Logger::verbose("butt", butt);

			if (butt == pin_butt3 && way == 0)
				way = 1;

			else if (butt != pin_butt3 && butt != pin_butt3 && way == 0)

				way = 0;

			if (butt == pin_butt2 && way == 1)
				way = 2;
			else if (butt != pin_butt2 && butt != pin_butt3 && way == 1)
				way = 0;

			if (butt == pin_butt4 && way == 2)
				way = 3;
			else if (butt != pin_butt2 && butt != pin_butt4 && way == 2)
				way = 0;

			if (butt == pin_butt2 && way == 3)
				way = 4;
			else if (butt != pin_butt2 && butt != pin_butt4 && way == 3)
				way = 0;

			if (butt == pin_butt3 && way == 4)
				way = 5;
			else if (butt != pin_butt3 && butt != pin_butt2 && way == 4)
				way = 0;

			if (butt == pin_butt1 && way == 5)
				way = 6;
			else if (butt != pin_butt3 && butt != pin_butt1 && way == 5)
				way = 0;

			SEFL::Logger::verbose("way", way);
		}

		//	SEFL::Logger::verbose("elevatorInSwith", Mext.digitalRead(elevatorInSwith));
		//					SEFL::Logger::verbose("elevatorOutSwith", Mext.digitalRead(elevatorOutSwith));

		if (!Mext.digitalRead(elevatorInSwith) && !Mext.digitalRead(elevatorOutSwith))
		{

			for (int i = 0; i < NUMPIXELS; i++)
			{ // For each pixel...
				labyrintpixels.setPixelColor(i, labyrintpixels.Color(120, 120, 60));
			}

			const int way1[5] = {1, 10, 13, 22, 22};
			const int way2[5] = {22, 21, 20, 20, 20};
			const int way3[5] = {20, 15, 8, 8, 8};
			const int way4[5] = {8, 7, 6, 6, 6};
			const int way5[5] = {6, 17, 18, 29, 30};
			const int way6[5] = {30, 31, 32, 33, 33};

			for (int i = 0; i < 5; i++)
			{
				labyrintpixels.setPixelColor(way1[i], 0, 40, 0);
				labyrintpixels.setPixelColor(way2[i], 0, 40, 0);
				labyrintpixels.setPixelColor(way3[i], 0, 40, 0);
				labyrintpixels.setPixelColor(way4[i], 0, 40, 0);
				labyrintpixels.setPixelColor(way5[i], 0, 40, 0);
				labyrintpixels.setPixelColor(way6[i], 0, 40, 0);
			}

			if (way == 1)
			{
				for (int i = 0; i < 5; i++)
				{
					labyrintpixels.setPixelColor(way1[i], 0, 0, 255);
				}
			}

			if (way == 2)
			{
				for (int i = 0; i < 5; i++)
				{
					labyrintpixels.setPixelColor(way1[i], 0, 0, 255);
					labyrintpixels.setPixelColor(way2[i], 0, 0, 255);
				}
			}

			if (way == 3)
			{
				for (int i = 0; i < 5; i++)
				{
					labyrintpixels.setPixelColor(way1[i], 0, 0, 255);
					labyrintpixels.setPixelColor(way2[i], 0, 0, 255);
					labyrintpixels.setPixelColor(way3[i], 0, 0, 255);
				}
			}

			if (way == 4)
			{
				for (int i = 0; i < 5; i++)
				{
					labyrintpixels.setPixelColor(way1[i], 0, 0, 255);
					labyrintpixels.setPixelColor(way2[i], 0, 0, 255);
					labyrintpixels.setPixelColor(way3[i], 0, 0, 255);
					labyrintpixels.setPixelColor(way4[i], 0, 0, 255);
				}
			}

			if (way == 5)
			{
				for (int i = 0; i < 5; i++)
				{
					labyrintpixels.setPixelColor(way1[i], 0, 0, 255);
					labyrintpixels.setPixelColor(way2[i], 0, 0, 255);
					labyrintpixels.setPixelColor(way3[i], 0, 0, 255);
					labyrintpixels.setPixelColor(way4[i], 0, 0, 255);
					labyrintpixels.setPixelColor(way5[i], 0, 0, 255);
				}
			}

			if (way == 6)
			{
				for (int i = 0; i < 5; i++)
				{
					labyrintpixels.setPixelColor(way1[i], 0, 0, 255);
					labyrintpixels.setPixelColor(way2[i], 0, 0, 255);
					labyrintpixels.setPixelColor(way3[i], 0, 0, 255);
					labyrintpixels.setPixelColor(way4[i], 0, 0, 255);
					labyrintpixels.setPixelColor(way5[i], 0, 0, 255);
					labyrintpixels.setPixelColor(way6[i], 0, 0, 255);
				}
			}

			//		if (way == 0) {
			//			for (int i = 0; i < NUMPIXELS; i++) { // For each pixel...
			//				labyrintpixels.setPixelColor(i,
			//						labyrintpixels.Color(120, 120, 20));
			//
			//				//	    pixels.show();   // Send the updated pixel colors to the hardware.
			//				//	    delay(DELAYVAL); // Pause before next pass through loop
			//		//	}
			//		}
			//
			//	}

			labyrintpixels.show(); // Send the updated pixel colors to the hardware.
		}

		if (Mext.digitalRead(elevatorInSwith) || Mext.digitalRead(elevatorOutSwith))
		{

			labyrintpixels.clear(); // Set all pixel colors to 'off'
			labyrintpixels.show();
		}

		prevbutt = butt;

		if (way == 6)
		{
			for (int i = 0; i < NUMPIXELS; i++)
			{ // For each pixel...
				labyrintpixels.setPixelColor(i, labyrintpixels.Color(20, 20, 20));
			}
			labyrintpixels.show(); // Send the updated pixel colors to the hardware.

			if (!flag_finish)
			{
				flag_finish = true;
				timer = millis();

				if (player != nullptr)
				{
					player->play(5);
					SEFL::Logger::verbose("Labyrinth", "0005.mp3");
				}
				in_magnet->setStatus(SEFL::ActuatorClientStatuses::ON_STATUS);
			}
		}
		if (flag_finish && millis() - timer > 7000)
		{
			for (int i = 0; i < NUMPIXELS; i++)
			{ // For each pixel...
				labyrintpixels.setPixelColor(i, labyrintpixels.Color(0, 0, 0));
			}
			const int exit[8] = {3, 8, 15, 20, 27, 32, 26, 28};
			for (int i = 0; i < 8; i++)
			{
				labyrintpixels.setPixelColor(exit[i], 0, 150, 0);
			}
			labyrintpixels.show(); // Send the updated pixel colors to the hardware.
			this->setStatus(BasicClientStatuses::FINISHED_STATUS);
		}
	}

	void Labyrinth::onFinished()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();

			SEFL::Logger::verbose("Labyrinth", "onFinished");
			//	labyrintpixels.clear(); // Set all pixel colors to 'off'
			//	labyrintpixels.show();
		}
	}

	void Labyrinth::onManualFinished()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();

			SEFL::Logger::verbose("Labyrinth", "onManualFinished");
			//	labyrintpixels.clear(); // Set all pixel colors to 'off'
			//	labyrintpixels.show();
		}
	}

	void Labyrinth::onDefault()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();

			SEFL::Logger::verbose("Labyrinth", "onDefault");
			labyrintpixels.clear(); // Set all pixel colors to 'off'
			labyrintpixels.show();
		}
	}

	uint8_t Labyrinth::getLabyrinthAddressLed()
	{
		return labyrinthAddressLed;
	}

	void Labyrinth::setLabyrinthAddressLed(uint8_t labyrinthAddressLed)
	{
		this->labyrinthAddressLed = labyrinthAddressLed;
	}

	uint8_t Labyrinth::getLabyrinthLeftButton()
	{
		return labyrinthLeftButton;
	}

	void Labyrinth::setLabyrinthLeftButton(uint8_t labyrinthLeftButton)
	{
		this->labyrinthLeftButton = labyrinthLeftButton;
	}

	uint8_t Labyrinth::getLabyrinthRightButton()
	{
		return labyrinthRightButton;
	}

	void Labyrinth::setLabyrinthRightButton(uint8_t labyrinthRightButton)
	{
		this->labyrinthRightButton = labyrinthRightButton;
	}

	DFRobotDFPlayerMini *Labyrinth::getPlayer()
	{
		return player;
	}

	uint8_t Labyrinth::getElevatorInSwith()
	{
		return elevatorInSwith;
	}

	void Labyrinth::setElevatorInSwith(uint8_t elevatorInSwith)
	{
		this->elevatorInSwith = elevatorInSwith;
	}

	uint8_t Labyrinth::getElevatorOutSwith()
	{
		return elevatorOutSwith;
	}

	uint8_t Labyrinth::getLabyrinthDownButton()
	{
		return labyrinthDownButton;
	}

	void Labyrinth::setLabyrinthDownButton(uint8_t labyrinthDownButton)
	{
		this->labyrinthDownButton = labyrinthDownButton;
	}

	uint8_t Labyrinth::getLabyrinthUpButton()
	{
		return labyrinthUpButton;
	}

	bool Labyrinth::isFlagFinish()
	{
		return flag_finish;
	}

	Magnet *Labyrinth::getInMagnet()
	{
		return in_magnet;
	}

	void Labyrinth::setInMagnet(Magnet *inMagnet)
	{
		in_magnet = inMagnet;
	}

	Magnet *Labyrinth::getOutMagnet()
	{
		return out_magnet;
	}

	void Labyrinth::setOutMagnet(Magnet *outMagnet)
	{
		out_magnet = outMagnet;
	}

	void Labyrinth::setFlagFinish(bool flagFinish)
	{
		flag_finish = flagFinish;
	}

	uint64_t Labyrinth::getTimer()
	{
		return timer;
	}

	void Labyrinth::setTimer(uint64_t timer)
	{
		this->timer = timer;
	}

	void Labyrinth::setLabyrinthUpButton(uint8_t labyrinthUpButton)
	{
		this->labyrinthUpButton = labyrinthUpButton;
	}

	void Labyrinth::setElevatorOutSwith(uint8_t elevatorOutSwith)
	{
		this->elevatorOutSwith = elevatorOutSwith;
	}

	void Labyrinth::setPlayer(DFRobotDFPlayerMini *player)
	{
		this->player = player;
	}

	Labyrinth::~Labyrinth()
	{
		// TODO Auto-generated destructor stub
	}

} /* namespace SEFL */

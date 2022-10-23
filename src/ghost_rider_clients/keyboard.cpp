/*
 * keyboard.cpp
 *
 *  Created on: Jan 31, 2022
 *      Author: piatk
 */

#include "DFRobotDFPlayerMini.h"
#include "keyboard.h"
uint8_t prevkey = 0;
uint8_t pin;

uint8_t pin_key1 = 1;
uint8_t pin_key2 = 9;
uint8_t pin_key3 = 8;
uint8_t pin_key4 = 4;

namespace SEFL
{

	Keyboard::Keyboard(MQTTClient &mqtt, const char *name, uint8_t reset_status,
					   const char *placement, const char *in_topic, const char *out_topic,
					   SEFL::Language language) : Quest_Basic_Client(mqtt, name, reset_status, placement, in_topic,
																	 out_topic, language)
	{

		keyboardPin = 0;
		keyboardMagnet = 0;
		fireplaceSmokeRelay = 1;
		player = nullptr;
	}

	void Keyboard::onActive()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("Keyboard", "onActive");

			Pext.digitalWrite(keyboardMagnet, true);

			uint8_t pin = 0;
		}

		//
		//  SEFL::Logger::verbose("Keyboard",Mext.analogRead(keyboardPin));
		int keyboard;
		uint8_t key;

		keyboard = Mext.analogRead(keyboardPin);
		SEFL::Logger::verbose("Keyboard", keyboard);

		if (keyboard > 1000)
			key = 1;
		else if (keyboard > 900)
			key = 2;
		else if (keyboard > 800)
			key = 3;
		else if (keyboard > 750)
			key = 4;
		else if (keyboard > 700)
			key = 5;
		else if (keyboard > 660)
			key = 6;
		else if (keyboard > 620)
			key = 7;
		else if (keyboard > 580)
			key = 8;
		else if (keyboard > 560)
			key = 9;
		else if (keyboard > 530)
			key = 10;
		else if (keyboard > 500)
			key = 0;
		else if (keyboard > 470)
			key = 11;
		else
			key = 12;

		//		if (prevkey != key && key !=12)	{
		//		//	SEFL::Logger::verbose("Keyboard key",key);
		//
		//			if (key == pin_key1 && pin == 0) pin = 1;
		//			if (key == pin_key2 && pin == pin_key1) pin = 2;
		//
		//			if  (key != pin_key2 && key != pin_key1 && pin == 1) pin = 0;
		//			if  (key != pin_key2 && key != pin_key3 && pin == 2) pin = 0;
		//			if(key == pin_key3 && pin == 2) pin = 3;
		//	//		SEFL::Logger::verbose("Keyboard pin count",pin);
		//		}
		//
		//---------------------------------------------------------------------
		if (prevkey != key && key != 12)
		{
			if (key == pin_key4 && pin == 3)
				pin = 4;
			if (key != pin_key4 && pin == 3)
				pin = 0;

			if (key == pin_key3 && pin == 2)
				pin = 3;
			if (key != pin_key3 && pin == 2)
				pin = 0;

			if (key == pin_key2 && pin == 1)
				pin = 2;
			if (key != pin_key2 && pin == 1)
				pin = 0;

			if (key == pin_key1 && pin == 0)
				pin = 1;

			SEFL::Logger::verbose("Keyboard pin count", pin);
		}

		prevkey = key;
		if (pin == 4)
		{
			Pext.digitalWrite(fireplaceSmokeRelay, false);
			this->setStatus(BasicClientStatuses::FINISHED_STATUS);
		}
	}

	void Keyboard::onFinished()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("Keyboard", "onFinished");
			Pext.digitalWrite(fireplaceSmokeRelay, false);

			pin = 0;
		}
	}

	void Keyboard::onManualFinished()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("Keyboard", "onManualFinished");
			Pext.digitalWrite(fireplaceSmokeRelay, false);

			pin = 0;
		}
	}

	void Keyboard::onDefault()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("Keyboard", "onDefault");
			Pext.digitalWrite(keyboardMagnet, false);
			Pext.digitalWrite(fireplaceSmokeRelay, true);
			player->stop();

			pin = 0;
		}
	}

	uint8_t Keyboard::getKeyboardMagnet()
	{
		return keyboardMagnet;
	}

	void Keyboard::setKeyboardMagnet(uint8_t keyboardMagnet)
	{
		this->keyboardMagnet = keyboardMagnet;
	}

	uint8_t Keyboard::getKeyboardPin()
	{
		return keyboardPin;
	}

	void Keyboard::setKeyboardPin(uint8_t keyboardPin)
	{
		this->keyboardPin = keyboardPin;
	}

	DFRobotDFPlayerMini *Keyboard::getPlayer()
	{
		return player;
	}

	uint8_t Keyboard::getFireplaceSmokeRelay()
	{
		return fireplaceSmokeRelay;
	}

	void Keyboard::setFireplaceSmokeRelay(uint8_t fireplaceSmokeRelay)
	{
		this->fireplaceSmokeRelay = fireplaceSmokeRelay;
	}

	void Keyboard::setPlayer(DFRobotDFPlayerMini *player)
	{
		this->player = player;
	}

	Keyboard::~Keyboard()
	{
		// TODO Auto-generated destructor stub
	}

} /* namespace SEFL */

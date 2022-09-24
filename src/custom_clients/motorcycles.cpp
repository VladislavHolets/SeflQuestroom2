/*
 * motorcycles.cpp
 *
 *  Created on: Jan 31, 2022
 *      Author: piatk
 */

#include <Adafruit_NeoPixel.h>
#include <custom_clients/motorcycles.h>
#include <PinNames.h>
#include <utils/logger.h>
#include <peripherals/multpinsextender.h>
#include <peripherals/pwmpinsextender.h>
#include <wiring_constants.h>
#include <wiring_digital.h>
#include <WMath.h>

namespace SEFL
{

	uint8_t PIN = PB_2; ////;5  //  PB_2
#define NUMPIXELS 27	//

	Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 50 // Time (in milliseconds) to pause between pixels

	Motorcycles::Motorcycles(MQTTClient &mqtt, const char *name, uint8_t reset_status,
							 const char *placement, const char *in_topic, const char *out_topic,
							 SEFL::Language language) : Quest_Basic_Client(mqtt, name, reset_status, placement, in_topic,
																		   out_topic, language)
	{

		pot_l = 0;
		pot_c = 1;
		pot_r = 2;
		arduino_in = 3;
		motorcyclesAddressLed = 0;
		arduino_power = 1;
		player = nullptr;
		pixels_ = nullptr;
	}

	Motorcycles::~Motorcycles()
	{
		// TODO Auto-generated destructor stub
	}

	void Motorcycles::onActive()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("Motorcycles", "onActive");
			Mext.digitalRead(arduino_in);
			PIN = Mext.getCi(); // motorcyclesAddressLed;
			pinMode(Mext.getCi(), INPUT_PULLUP);
			Pext.digitalWrite(arduino_power, LOW);

			//	PIN = Mext.getCi(); //motorcyclesAddressLed;
			//	Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

			//		pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
		}
		//
		//	if (this->isChangedStatus()) {
		//	}
		//
		//	uint8_t pot1 = map(Mext.analogRead(pot_l), 0, 900, 0, 9);
		//	if (pot1 > 9) pot1=9;
		//	uint8_t pot2 = map(Mext.analogRead(pot_c), 0, 900, 0, 9);
		//	if (pot2 > 9) pot2=9;
		//	uint8_t pot3 = map(Mext.analogRead(pot_r), 0, 900, 0, 9);
		//	if (pot3 > 9) pot3=9;
		//
		//
		//
		//
		//
		//	SEFL::Logger::verbose("Motorcycles", pot1);
		//	SEFL::Logger::verbose("Motorcycles    ", pot2);
		//	SEFL::Logger::verbose("Motorcycles       ", pot3);
		//
		//	SEFL::Logger::verbose("Motorcycles           ", Mext.analogRead(pot_l));
		//	SEFL::Logger::verbose("Motorcycles                      ",
		//			Mext.analogRead(pot_c));
		//	SEFL::Logger::verbose("Motorcycles                                    ",
		//			Mext.analogRead(pot_r));
		//
		//	for (int i = 0; i < NUMPIXELS; i++) { // For each pixel...
		//		pixels.setPixelColor(i, pixels.Color(160, 20, 20));
		//		//	    pixels.show();   // Send the updated pixel colors to the hardware.
		//		//	    delay(DELAYVAL); // Pause before next pass through loop
		//	}
		//
		//	if (pot1 > 0) {
		//
		//		for (int i = pot1 * 3 - 3; i < pot1 * 3; i++) { // For each pixel...
		//			pixels.setPixelColor(i, pixels.Color(0, 255, 0));
		//		}
		//
		//	}
		//	if (pot2 > 0) {
		//		for (int i = pot2 * 3 - 3; i < pot2* 3; i++) { // For each pixel...
		//			pixels.setPixelColor(i, pixels.Color(255, 255, 0));
		//		}
		//
		//	}
		//
		//	if (pot3 > 0) {
		//		for (int i = pot3 * 3 - 3; i < pot3 * 3; i++) { // For each pixel...
		//			pixels.setPixelColor(i, pixels.Color(0, 0, 255));
		//		}
		//	}
		//
		//	pixels.show();   // Send the updated pixel colors to the hardware.
		//

		//	if (pot1 == 8 && pot2 == 9 && pot3 == 5) {
		//		this->setStatus(BasicClientStatuses::FINISHED_STATUS);
		//	}

		if (!Mext.digitalRead(arduino_in))
		{
			this->setStatus(BasicClientStatuses::FINISHED_STATUS);
		}
	}

	void Motorcycles::onFinished()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("Motorcycles", "onFinished");
			pixels.clear(); // Set all pixel colors to 'off'
			pixels.show();
		}
	}

	void Motorcycles::onManualFinished()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("Motorcycles", "onManualFinished");
			pixels.clear(); // Set all pixel colors to 'off'
			pixels.show();
		}
	}

	void Motorcycles::onDefault()
	{

		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("Motorcycles", "onDefault");
			pixels.clear(); // Set all pixel colors to 'off'
			pixels.show();
			Pext.digitalWrite(arduino_power, HIGH);
		}
	}

	uint8_t Motorcycles::getMotorcyclesAddressLed()
	{
		return motorcyclesAddressLed;
	}

	void Motorcycles::setMotorcyclesAddressLed(uint8_t motorcyclesAddressLed)
	{
		this->motorcyclesAddressLed = motorcyclesAddressLed;
	}

	DFRobotDFPlayerMini *Motorcycles::getPlayer()
	{
		return player;
	}

	void Motorcycles::setPlayer(DFRobotDFPlayerMini *player)
	{
		this->player = player;
	}

	uint8_t Motorcycles::getPotC()
	{
		return pot_c;
	}

	void Motorcycles::setPotC(uint8_t potC)
	{
		pot_c = potC;
	}

	uint8_t Motorcycles::getPotL()
	{
		return pot_l;
	}

	void Motorcycles::setPotL(uint8_t potL)
	{
		pot_l = potL;
	}

	uint8_t Motorcycles::getPotR()
	{
		return pot_r;
	}

	Adafruit_NeoPixel *&Motorcycles::getPixels()
	{
		return pixels_;
	}

	uint8_t Motorcycles::getArduinoIn()
	{
		return arduino_in;
	}

	uint8_t Motorcycles::getArduinoPower()
	{
		return arduino_power;
	}

	void Motorcycles::setArduinoPower(uint8_t arduinoPower)
	{
		arduino_power = arduinoPower;
	}

	void Motorcycles::setArduinoIn(uint8_t arduinoIn)
	{
		arduino_in = arduinoIn;
	}

	void Motorcycles::setPixels(Adafruit_NeoPixel *&pixels)
	{
		pixels_ = pixels;
	}

	void Motorcycles::setPotR(uint8_t potR)
	{
		pot_r = potR;
	}

} /* namespace SEFL */

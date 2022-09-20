/*
 * pwmpinsextender.h
 *
 *  Created on: May 6, 2021
 *      Author: vladi
 */

#ifndef PWMPINSEXTENDER_H_
#define PWMPINSEXTENDER_H_

#include <binary.h>
#include <PCA9685.h>
#include <sys/_stdint.h>
#include <wiring_constants.h>
#include <Wire.h>

class TwoWire;

namespace SEFL
{

	class PWM_Pins_Extender
	{
		static uint16_t const MAX_PWM = 4096;
		int trashhold_;
		const byte _i2cAddress;
		TwoWire *_i2cWire; // Wire class instance (unowned) (default: Wire)
		const uint32_t _i2cSpeed;

		PCA9685 *handler_;

	public:
		void refresh();
		uint16_t pins_values[16];
		PWM_Pins_Extender(byte i2cAddress = B000000, TwoWire &i2cWire = Wire,
						  uint32_t i2cSpeed = 400000);
		PWM_Pins_Extender(PCA9685 &handler, int16_t trashhold = 2048);
		void digitalWrite(uint8_t pin, uint8_t val);
		bool digitalRead(uint8_t pin, int16_t trashhold = -1);
		void analogWrite(uint8_t pin, uint16_t val);
		int16_t analogRead(uint8_t pin);
		byte getI2cAddress();
		uint32_t getI2cSpeed();
		TwoWire *&getI2cWire();
		PCA9685 *&getHandler();
		int getTrashhold() const;
		void setTrashhold(int trashhold_);
	};
} /* namespace SEFL */

extern SEFL::PWM_Pins_Extender Pext;
#endif /* PWMPINSEXTENDER_H_ */

/*
 * pwmpinsextender.cpp
 *
 *  Created on: May 6, 2021
 *      Author: vladi
 */

#include "../constants.h"
#include "pwmpinsextender.h"
#include "../variants/blackpill_variant.hpp"

namespace SEFL
{

} /* namespace SEFL */

SEFL::PWM_Pins_Extender::PWM_Pins_Extender(byte i2cAddress, TwoWire &i2cWire,
										   uint32_t i2cSpeed) : _i2cAddress(i2cAddress), _i2cWire(&i2cWire), _i2cSpeed(i2cSpeed)
{
	this->handler_ = new PCA9685(this->_i2cAddress, i2cWire, this->_i2cSpeed);
	this->trashhold_ = this->MAX_PWM / 2;

}

SEFL::PWM_Pins_Extender::PWM_Pins_Extender(PCA9685 &handler, int16_t trashhold) : trashhold_((trashhold) ? trashhold : this->MAX_PWM / 2), _i2cAddress(
																																			   handler.getI2CAddress()),
																				  _i2cWire(nullptr), _i2cSpeed(
																										 handler.getI2CSpeed()),
																				  handler_(&handler)
{
}

void SEFL::PWM_Pins_Extender::digitalWrite(uint8_t pin, uint8_t val)
{
	if (pin > 15)
		return;
	pins_values[pin] = (val != 0) ? 4096 : 0;
	if (val)
		this->handler_->setChannelOn(pin);
	else
		this->handler_->setChannelOff(pin);
}

bool SEFL::PWM_Pins_Extender::digitalRead(uint8_t pin, int16_t trashhold)
{
	if (pin > 15)
		return (false);
	if (trashhold >= 0)
	{
		return ((this->pins_values[pin] >= trashhold) ? HIGH : LOW);
	}
	else
	{
		return ((this->pins_values[pin] >= this->getTrashhold()) ? HIGH : LOW);
	}
}

void SEFL::PWM_Pins_Extender::analogWrite(uint8_t pin, uint16_t val)
{
	if (pin > 15)
		return;

	pins_values[pin] = (val > 4095) ? 4095 : val;
	this->handler_->setChannelPWM(pin, pins_values[pin]);
}

int16_t SEFL::PWM_Pins_Extender::analogRead(uint8_t pin)
{
	if (pin > 15)
		return (-1);
	return (this->pins_values[pin]);
}

byte SEFL::PWM_Pins_Extender::getI2cAddress()
{
	return (_i2cAddress);
}

uint32_t SEFL::PWM_Pins_Extender::getI2cSpeed()
{
	return (_i2cSpeed);
}

TwoWire *&SEFL::PWM_Pins_Extender::getI2cWire()
{
	return (_i2cWire);
}

PCA9685 *&SEFL::PWM_Pins_Extender::getHandler()
{
	return (handler_);
}

int SEFL::PWM_Pins_Extender::getTrashhold()
{
	return (trashhold_);
}

void SEFL::PWM_Pins_Extender::setTrashhold(int trashhold)
{
	trashhold_ = trashhold;
}

void SEFL::PWM_Pins_Extender::refresh()
{
	this->handler_->setChannelsPWM(0, 15, &(this->pins_values[0]));
}

SEFL::PWM_Pins_Extender Pext(SEFL::pext_config.i2cAddress, *(SEFL::pext_config.i2cWire), SEFL::pext_config.i2cSpeed);

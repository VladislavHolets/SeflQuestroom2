/*
 * hc595driver.cpp
 *
 *  Created on: Aug 16, 2021
 *      Author: vladi
 */

#include <peripherals/hc595driver.h>

namespace SEFL
{

} /* namespace SEFL */

void SEFL::HC595_Driver::shiftOut(uint16_t length)
{
	if (length > HC595_BUFFER_SIZE)
	{
		length = HC595_BUFFER_SIZE;
	}
	digitalWrite(cfg_.data_pin, 0);
	digitalWrite(cfg_.clock_pin, 0);
	int pinState;
	for (int l = 0; l < length; l++)
	{
		for (int i = 0; i < 8; i++)
		{
			digitalWrite(cfg_.clock_pin, 0);
			if (data[l] & (1 << (7 - i + 0)))
			{
				pinState = 1;
			}
			else
			{
				pinState = 0;
			}
			digitalWrite(cfg_.data_pin, pinState);
			digitalWrite(cfg_.clock_pin, 1);
			digitalWrite(cfg_.data_pin, 0);
		}
	}

	digitalWrite(cfg_.clock_pin, 0);
}

void SEFL::HC595_Driver::setOrder(BitOrder order_)
{
	this->order_ = order_;
}

SEFL::HC595_Driver::HC595_Driver(HC595_cfg cfg) : cfg_{cfg.data_pin, cfg.clock_pin, cfg.latch_pin, cfg.chip_amount}, data{
																														 0},
												  order_(MSBFIRST)
{
	if (this->cfg_.chip_amount > HC595_BUFFER_SIZE)
	{
		this->cfg_.chip_amount = HC595_BUFFER_SIZE;
	}
	// spi.setClockDivider(1);
	// spi.begin();
	pinMode(cfg.latch_pin, OUTPUT);
	pinMode(cfg.data_pin, OUTPUT);
	pinMode(cfg.clock_pin, OUTPUT);

	digitalWrite(cfg.latch_pin, HIGH);
}

SEFL::HC595_Driver::HC595_Driver(uint8_t data_pin, uint8_t clock_pin,
								 uint8_t latch_pin, uint16_t chip_amount) : cfg_{data_pin, clock_pin, latch_pin, chip_amount}, data{0}, order_(MSBFIRST), spi(data_pin, PA6, clock_pin, NC)
{
	if (this->cfg_.chip_amount > HC595_BUFFER_SIZE)
	{
		this->cfg_.chip_amount = HC595_BUFFER_SIZE;
	}
	// spi.setClockDivider(1);
	spi.begin();
	pinMode(cfg_.latch_pin, OUTPUT);
	digitalWrite(cfg_.latch_pin, HIGH);
}

SEFL::HC595_Driver::~HC595_Driver()
{
	spi.end();
}

void SEFL::HC595_Driver::setData(uint16_t item, uint8_t state)
{
	if (item >= this->cfg_.chip_amount * 8)
	{
		return;
	}
	this->data[item / 8] = this->data[item / 8] | state << (item % 8);
}

void SEFL::HC595_Driver::cleanData()
{
	for (int i = 0; i < this->cfg_.chip_amount; i++)
	{
		this->data[i] = 0;
	}
}

void SEFL::HC595_Driver::setData(uint8_t *data, uint16_t length,
								 uint16_t start_byte)
{
	if ((start_byte + length) > this->cfg_.chip_amount)
	{
		return;
	}
	memcpy(this->data + start_byte, data, length);
}

void SEFL::HC595_Driver::sendData(uint16_t length)
{
	digitalWrite(this->cfg_.latch_pin, LOW);
	shiftOut(length);
	digitalWrite(this->cfg_.latch_pin, HIGH);
}

/*
 * hc595driver.h
 *
 *  Created on: Aug 16, 2021
 *      Author: vladi
 */

#ifndef HC595DRIVER_H_
#define HC595DRIVER_H_

#include "../variants/blackpill_variant.hpp"
#include <sys/_stdint.h>
#include <SPI.h>
#include <wiring_constants.h>

namespace SEFL
{
	struct HC595_cfg
	{
		uint8_t data_pin;
		uint8_t clock_pin;
		uint8_t latch_pin;
		uint16_t chip_amount;
	};

	class HC595_Driver
	{
		SPIClass spi;
		BitOrder order_;
		HC595_cfg cfg_;
		uint8_t data[HC595_BUFFER_SIZE];
		void shiftOut(uint16_t length);

	public:
		HC595_Driver(HC595_cfg cfg);
		HC595_Driver(uint8_t data_pin, uint8_t clock_pin, uint8_t latch_pin, uint16_t chip_amount = HC595_BUFFER_SIZE);
		virtual ~HC595_Driver();
		void setData(uint16_t item, uint8_t state);
		void cleanData();
		void setData(uint8_t *data, uint16_t length = HC595_BUFFER_SIZE, uint16_t start_byte = 0);
		void sendData(uint16_t length = HC595_BUFFER_SIZE);
		void setOrder(BitOrder order_);
	};

} /* namespace SEFL */

#endif /* HC595DRIVER_H_ */

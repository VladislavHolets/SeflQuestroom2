/*
 * blackpill_variant.hpp
 *
 *  Created on: Jun 30, 2021
 *      Author: vladi
 */

#ifndef SEFL_VARIANTS_BLACKPILL_VARIANT_HPP_
#define SEFL_VARIANTS_BLACKPILL_VARIANT_HPP_

#include <sys/_stdint.h>
#include <wiring_constants.h>
#include <Wire.h>
#define MEM_TYPE PROGMEM
namespace SEFL {
const uint16_t HC595_BUFFER_SIZE=10;
struct PEXT_Config {
	byte i2cAddress;
	TwoWire *i2cWire;
	uint32_t i2cSpeed;
};
static const PEXT_Config pext_config = { 0x40, &Wire, 400000 };

struct MEXT_Config {
	uint8_t s0;
	uint8_t s1;
	uint8_t s2;
	uint8_t s3;
	uint8_t cl;
};
static const MEXT_Config mext_config = { PA_8, PA_9, PA_10, PA_15, PA_0 };
struct UEXT_Config {
	uint8_t TXM;
	uint8_t RXM;
	uint8_t SCL;
	uint8_t SDA;
	uint8_t MISO;
	uint8_t MOSI;
	uint8_t SCLK;
	uint8_t SSEL;
};
static const UEXT_Config uext_config = { PA_2, PA_3, PB_6, PB_7, PA_6, PA_7,
		PA_5, PB_2 };
}
#endif /* SEFL_VARIANTS_BLACKPILL_VARIANT_HPP_ */

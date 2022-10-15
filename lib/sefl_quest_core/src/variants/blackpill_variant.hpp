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

// #if !(defined(ARDUINO_BLACK_F407VE) || defined(ARDUINO_BLACK_F407VG) || defined(ARDUINO_BLACK_F407ZE) || defined(ARDUINO_BLACK_F407ZG) ||    \
// 	  defined(ARDUINO_BLUE_F407VE_Mini) || defined(ARDUINO_DIYMORE_F407VGT) || defined(ARDUINO_FK407M1) || defined(ARDUINO_NUCLEO_F429ZI) || \
// 	  defined(ARDUINO_DISCO_F746NG) || defined(ARDUINO_NUCLEO_F746ZG) || defined(ARDUINO_NUCLEO_F756ZG) || defined(ARDUINO_NUCLEO_H743ZI))
// //#error This code is designed to run on some STM32F407XX NUCLEO-F429ZI, STM32F746 and STM32F756 platform! Please check your Tools->Board setting.
// #warning Must use with STM32 core v2.2.0- or error
// #endif

#define MEM_TYPE PROGMEM
namespace SEFL
{
	const uint16_t HC595_BUFFER_SIZE = 10;
	struct PEXT_Config
	{
		byte i2cAddress;
		TwoWire *i2cWire;
		uint32_t i2cSpeed;
	};
	static const PEXT_Config pext_config = {0x40, &Wire, 400000};

	struct MEXT_Config
	{
		uint8_t s0;
		uint8_t s1;
		uint8_t s2;
		uint8_t s3;
		uint8_t cl;
	};
	static const MEXT_Config mext_config = {PA_8, PA_9, PA_10, PA_15, PA_0};
	struct UEXT_Config
	{
		uint8_t TXM;
		uint8_t RXM;
		uint8_t SCL;
		uint8_t SDA;
		uint8_t MISO;
		uint8_t MOSI;
		uint8_t SCLK;
		uint8_t SSEL;
	};
	static const UEXT_Config uext_config = {PA_2, PA_3, PB_6, PB_7, PA_6, PA_7,
											PA_5, PB_2};
}
#endif /* SEFL_VARIANTS_BLACKPILL_VARIANT_HPP_ */

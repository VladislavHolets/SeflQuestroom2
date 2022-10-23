/*
 * rfid.h
 *
 *  Created on: Nov 13, 2020
 *      Author: vladi
 */

#ifndef RFID_H_
#define RFID_H_

#include "sefl_quest_core.h"

#include <sys/_stdint.h>

namespace SEFL
{

	class RFID : public Quest_Sensor_Client
	{
		RFID1 rfid;
		uint8_t pins_[6];
		enum RFID_PIN_NAMES
		{
			SDA_PIN_NUM = 0,
			SCK_PIN_NUM = 1,
			MOSI_PIN_NUM = 2,
			MISO_PIN_NUM = 3,
			IRQ_PIN_NUM = 4,
			RST_PIN_NUM = 5
		};

	public:
		RFID(MQTTClient &mqtt, uint8_t sda_pin, uint8_t sck_pin, uint8_t mosi_pin, uint8_t irq_pin, uint8_t rst_pin, uint8_t miso_pin, const char *placement = "ghost_busters", const char *name = "RFID", uint8_t reset_status = 2);
		virtual ~RFID();
		virtual void onReset() override;
		virtual void onSet() override;
	};

} /* namespace SEFL */

#endif /* RFID_H_ */

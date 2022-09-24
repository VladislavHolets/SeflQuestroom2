/*
 * rfid.cpp
 *
 *  Created on: Nov 13, 2020
 *      Author: vladi
 */

#include <custom_clients/rfid.h>
#include <constants.h>

namespace SEFL
{

	RFID::RFID(MQTTClient &mqtt, uint8_t sda_pin, uint8_t sck_pin,
			   uint8_t mosi_pin, uint8_t irq_pin, uint8_t rst_pin, uint8_t miso_pin,
			   const char *placement, const char *name, uint8_t reset_status) : Quest_Sensor_Client(mqtt, name, reset_status, placement)
	{
		this->pins_[SEFL::RFID::SDA_PIN_NUM] = sda_pin;
		this->pins_[SEFL::RFID::SCK_PIN_NUM] = sck_pin;
		this->pins_[SEFL::RFID::MOSI_PIN_NUM] = mosi_pin;
		this->pins_[SEFL::RFID::MISO_PIN_NUM] = miso_pin;
		this->pins_[SEFL::RFID::IRQ_PIN_NUM] = irq_pin;
		this->pins_[SEFL::RFID::RST_PIN_NUM] = rst_pin;
	}

	RFID::~RFID()
	{
		// TODO Auto-generated destructor stub
	}

	void RFID::onReset()
	{
		if (this->isChangedStatus())
		{
			this->unsetChangedStatus();
			this->reportStatus();
		}
		rfid.begin(this->pins_[IRQ_PIN_NUM], this->pins_[SCK_PIN_NUM], this->pins_[MOSI_PIN_NUM],
				   this->pins_[MISO_PIN_NUM], this->pins_[SDA_PIN_NUM], this->pins_[RST_PIN_NUM]);
		// delay(10);
		rfid.init();
		// delay(10);
		uint8_t status;
		uint8_t str[MAX_LEN];
		// Search card, return card types
		status = rfid.request(PICC_REQIDL, str);
		if (status == MI_OK)
		{
			this->setStatus(SEFL::SensorClientStatuses::SET_STATUS);
		}
		rfid.halt();
	}

	void RFID::onSet()
	{
		if (this->isChangedStatus())
		{
			this->unsetChangedStatus();
			this->reportStatus();
		}
	}

} /* namespace SEFL */

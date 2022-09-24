/*
 * magnet.cpp
 *
 *  Created on: Jan 21, 2022
 *      Author: piatk
 */

#include <custom_clients/magnet.h>
#include <constants.h>
#include <utils/logger.h>
#include <peripherals/pwmpinsextender.h>
#include <sys/_stdint.h>

namespace SEFL
{

	Magnet::Magnet(MQTTClient &mqtt, const char *name, uint8_t reset_status,
				   const char *placement, const char *in_topic, const char *out_topic,
				   SEFL::Language language) : Quest_Actuator_Client(mqtt, name, reset_status, placement, in_topic,
																	out_topic, language)
	{
		pin_number = 0;
		inverted = 0;
	}

	Magnet::~Magnet()
	{
		// TODO Auto-generated destructor stub
	}

	void Magnet::onOn()
	{
		if (this->changed_status_)
		{
			SEFL::Logger::notice("Magnet", "is on");
			Pext.digitalWrite(pin_number, !inverted);
			this->reportStatus();
			this->unsetChangedStatus();
		}
	}

	bool Magnet::isInverted() const
	{
		return inverted;
	}

	void Magnet::setInverted(bool inverted)
	{
		this->inverted = inverted;
	}

	uint8_t Magnet::getPinNumber() const
	{
		return pin_number;
	}

	void Magnet::setPinNumber(uint8_t pinNumber)
	{
		pin_number = pinNumber;
	}

	void Magnet::onOff()
	{
		if (this->changed_status_)
		{
			SEFL::Logger::notice("Magnet", "is off");
			Pext.digitalWrite(pin_number, inverted);
			this->reportStatus();
			this->unsetChangedStatus();
		}
	}

} /* namespace SEFL */

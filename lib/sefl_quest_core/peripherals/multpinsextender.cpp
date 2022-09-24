/*
 * multpinsextender.cpp
 *
 *  Created on: May 9, 2021
 *      Author: vladi
 */

#include <peripherals/multpinsextender.h>
#include <variants/blackpill_variant.hpp>
#include <wiring_analog.h>
#include <wiring_digital.h>
#include <wiring_time.h>

namespace SEFL
{

	uint8_t Mult_Pins_Extender::getCi()
	{
		return (ci);
	}

	uint8_t Mult_Pins_Extender::commutate(uint8_t pin)
	{
		uint32_t timestamp = millis();
		if (pin != getLastCommute())
		{
			last_commute = pin;
			for (uint8_t i = 0; i < 4; ++i)
			{
				::digitalWrite(this->s[i], (pin & (1 << i)) ? HIGH : LOW);
			}
		}
		while ((millis() - timestamp) < 2)
		{
		}
		return (ci);
	}

	Mult_Pins_Extender::Mult_Pins_Extender(uint8_t s0, uint8_t s1, uint8_t s2,
										   uint8_t s3, uint8_t ci) : ci(ci), s{s0, s1, s2, s3}, last_commute(0)
	{
		// pinMode(ci, INPUT);

		for (uint8_t i = 0; i < 4; ++i)
		{
			pinMode(s[i], OUTPUT);
		}
		commutate(last_commute);
	}

	void Mult_Pins_Extender::digitalWrite(uint8_t pin, uint8_t val)
	{
		if (pin > 15)
			return;

		::digitalWrite(commutate(pin), val);
	}

	bool Mult_Pins_Extender::digitalRead(uint8_t pin, int16_t trashhold)
	{
		if (pin > 15)
			return (false);

		if (trashhold < 0)
			return (checkDigitalInput(commutate(pin)));
		else
		{
			return ((::analogRead(commutate(pin))) > trashhold);
		}
	}

	void Mult_Pins_Extender::analogWrite(uint8_t pin, uint16_t val)
	{
		if (pin > 15)
			return;
		::analogWrite(commutate(pin), val);
	}

	int16_t Mult_Pins_Extender::analogRead(uint8_t pin)
	{
		if (pin > 15)
			return (-1);
		return (::analogRead(commutate(pin)));
	}

	bool Mult_Pins_Extender::checkDigitalInput(uint8_t pin, uint8_t expected)
	{
		uint8_t t_counter = 0, checks = 2;
		for (int i = 0; i < checks; i++)
		{
			if (::digitalRead(pin) == expected)
			{
				t_counter++;
			}
		}
		return (t_counter > (checks / 2));
	}

	uint8_t Mult_Pins_Extender::getLastCommute() const
	{
		return (last_commute);
	}

	Mult_Pins_Extender::~Mult_Pins_Extender()
	{
		// TODO Auto-generated destructor stub
	}

} /* namespace SEFL */
SEFL::Mult_Pins_Extender Mext(SEFL::mext_config.s0, SEFL::mext_config.s1, SEFL::mext_config.s2, SEFL::mext_config.s3, SEFL::mext_config.cl);

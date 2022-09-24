/*
 * contract.cpp
 *
 *  Created on: Jan 25, 2022
 *      Author: piatk
 */

#include <custom_clients/contract.h>
#include <constants.h>
#include <utils/logger.h>
#include <peripherals/multpinsextender.h>
#include <peripherals/pwmpinsextender.h>
#include <sys/_stdint.h>
#include <wiring_constants.h>
#include <wiring_digital.h>
#include <wiring_time.h>

namespace SEFL
{
	uint64_t timer;
	bool flag;
	Contract::Contract(MQTTClient &mqtt, const char *name, uint8_t reset_status,
					   const char *placement, const char *in_topic, const char *out_topic,
					   SEFL::Language language) : Quest_Basic_Client(mqtt, name, reset_status, placement, in_topic,
																	 out_topic, language)
	{
		hell_light = 0;
		hell_magnet = 1;
		swith_contract = 0;
		smoke_button = 2;
		invertet = 0;
	}

	Contract::~Contract()
	{
		// TODO Auto-generated destructor stub
	}

	void Contract::onActive()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("Contract", "onActive");

			Pext.digitalWrite(hell_light, !invertet);
			Pext.digitalWrite(smoke_button, invertet);
			timer = millis();
			flag = 0;

			// Pext.digitalWrite(hell_magnet, invertet);
		}

		if (millis() - timer > 100 && flag == 0)
		{
			Pext.digitalWrite(smoke_button, !invertet);
			flag = 1;
		}
		if (millis() - timer > 200 && flag == 1)
		{
			Pext.digitalWrite(smoke_button, invertet);
		}

		if (millis() - timer > 2000)
		{
			Pext.digitalWrite(hell_magnet, invertet);
		}

		Mext.digitalRead(swith_contract);
		pinMode(Mext.getCi(), INPUT_PULLUP);

		if (Mext.digitalRead(swith_contract) == !invertet)
		{
			this->setStatus(BasicClientStatuses::FINISHED_STATUS);
		}
	}

	void Contract::onFinished()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("Contract", "onFinished");
			timer = millis();
			Pext.digitalWrite(hell_light, invertet);

			//		Pext.digitalWrite(smoke_button, !invertet);
			//		delay(100);
			//		Pext.digitalWrite(smoke_button, invertet);
			//		delay(100);
			//		Pext.digitalWrite(smoke_button, !invertet);
			//		delay(100);
			//		Pext.digitalWrite(smoke_button, invertet);
		}
		if (millis() - timer > 300)
		{
			Pext.digitalWrite(hell_light, !invertet);
		}
		if (millis() - timer > 400)
		{
			Pext.digitalWrite(hell_light, invertet);
		}
		if (millis() - timer > 600)
		{
			Pext.digitalWrite(hell_light, !invertet);
		}
		if (millis() - timer > 700)
		{
			Pext.digitalWrite(hell_light, invertet);
		}
		if (millis() - timer > 900)
		{
			Pext.digitalWrite(hell_light, !invertet);
		}
	}

	void Contract::onManualFinished()
	{

		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("Contract", "onManualFinished");

			Pext.digitalWrite(hell_light, invertet);

			//		Pext.digitalWrite(smoke_button, !invertet);
			//		delay(100);
			//		Pext.digitalWrite(smoke_button, invertet);
			//		delay(100);
			//		Pext.digitalWrite(smoke_button, !invertet);
			//		delay(100);
			//		Pext.digitalWrite(smoke_button, invertet);
			timer = millis();
		}

		if (millis() - timer > 300)
		{
			Pext.digitalWrite(hell_light, !invertet);
		}
		if (millis() - timer > 400)
		{
			Pext.digitalWrite(hell_light, invertet);
		}
		if (millis() - timer > 600)
		{
			Pext.digitalWrite(hell_light, !invertet);
		}
		if (millis() - timer > 700)
		{
			Pext.digitalWrite(hell_light, invertet);
		}
		if (millis() - timer > 900)
		{
			Pext.digitalWrite(hell_light, !invertet);
		}
	}

	uint8_t Contract::getHellLight()
	{
		return hell_light;
	}

	void Contract::setHellLight(uint8_t hellLight)
	{
		hell_light = hellLight;
	}

	uint8_t Contract::getHellMagnet()
	{
		return hell_magnet;
	}

	void Contract::setHellMagnet(uint8_t hellMagnet)
	{
		hell_magnet = hellMagnet;
	}

	bool Contract::isInvertet()
	{
		return invertet;
	}

	void Contract::setInvertet(bool invertet)
	{
		this->invertet = invertet;
	}

	uint8_t Contract::getSmokeButton()
	{
		return smoke_button;
	}

	void Contract::setSmokeButton(uint8_t smokeButton)
	{
		smoke_button = smokeButton;
	}

	uint8_t Contract::getSwithContract()
	{
		return swith_contract;
	}

	void Contract::setSwithContract(uint8_t swithContract)
	{
		swith_contract = swithContract;
	}

	void Contract::onDefault()
	{

		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("Contract", "onDefault");

			Pext.digitalWrite(hell_light, invertet);
			Pext.digitalWrite(hell_magnet, !invertet);
			Pext.digitalWrite(smoke_button, invertet);
		}
	}

} /* namespace SEFL */

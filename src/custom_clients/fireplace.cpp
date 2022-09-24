/*
 * fireplace.cpp
 *
 *  Created on: Jan 31, 2022
 *      Author: piatk
 */

#include <DFRobotDFPlayerMini.h>
#include <custom_clients/fireplace.h>
#include <utils/logger.h>
#include <peripherals/multpinsextender.h>
#include <peripherals/pwmpinsextender.h>
#include <wiring_constants.h>
#include <wiring_digital.h>

namespace SEFL
{

	Fireplace::Fireplace(MQTTClient &mqtt, const char *name, uint8_t reset_status,
						 const char *placement, const char *in_topic, const char *out_topic,
						 SEFL::Language language) : Quest_Basic_Client(mqtt, name, reset_status, placement, in_topic,
																	   out_topic, language)
	{

		fireplaceAcLightRelay = 0;
		fireplaceSmokeRelay = 1;
		fireplaceCoolerOut = 2;
		fireplaceLedOut = 3;

		fireplaceButtont = 0;

		liquidLevelSensor = 1;
		LiquidLevelLed = 4;

		player = nullptr;
	}

	void Fireplace::onActive()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			if (player != nullptr)
			{
				player->play(1);
				player->next();
				SEFL::Logger::verbose("Fireplace", "0001.mp3");
			}
			SEFL::Logger::verbose("Fireplace", "onActive");

			Pext.digitalWrite(fireplaceSmokeRelay, false);
			Pext.digitalWrite(fireplaceAcLightRelay, false);
			Pext.digitalWrite(fireplaceCoolerOut, false);
			Pext.digitalWrite(fireplaceLedOut, false);

			Pext.digitalWrite(LiquidLevelLed, LOW);
		}

		Mext.digitalRead(fireplaceButtont);
		pinMode(Mext.getCi(), INPUT_PULLUP);
		SEFL::Logger::verbose("Fireplace", Mext.digitalRead(fireplaceButtont));

		if (!Mext.digitalRead(fireplaceButtont))
		{
			this->setStatus(BasicClientStatuses::FINISHED_STATUS);
		}
	}

	void Fireplace::onFinished()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("Fireplace", "onFinished");
			Pext.digitalWrite(fireplaceSmokeRelay, true);
			// Pext.digitalWrite(fireplaceAcLightRelay, true);
			Pext.digitalWrite(fireplaceCoolerOut, true);
			Pext.digitalWrite(fireplaceLedOut, true);
			// player->stop();
		}

		if (Mext.digitalRead(liquidLevelSensor) == LOW)
		{
			Pext.digitalWrite(LiquidLevelLed, HIGH);
		}
		else
		{
			Pext.digitalWrite(LiquidLevelLed, LOW);
		}
	}

	void Fireplace::onManualFinished()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("Fireplace", "onManualFinished");
			Pext.digitalWrite(fireplaceSmokeRelay, true);
			// Pext.digitalWrite(fireplaceAcLightRelay, true);
			Pext.digitalWrite(fireplaceCoolerOut, true);
			Pext.digitalWrite(fireplaceLedOut, true);
			// player->stop();
		}
		if (Mext.digitalRead(liquidLevelSensor) == LOW)
		{
			Pext.digitalWrite(LiquidLevelLed, HIGH);
		}
		else
		{
			Pext.digitalWrite(LiquidLevelLed, LOW);
		}
	}

	void Fireplace::onDefault()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("Fireplace", "onDefault");
			Pext.digitalWrite(fireplaceSmokeRelay, true);
			Pext.digitalWrite(fireplaceAcLightRelay, true);
			Pext.digitalWrite(fireplaceCoolerOut, true);
			Pext.digitalWrite(fireplaceLedOut, true);
			Pext.digitalWrite(LiquidLevelLed, LOW);
			player->stop();
		}
		SEFL::Logger::verbose("liquidLevelSensor", Mext.digitalRead(liquidLevelSensor));
		//	if (Mext.digitalRead(liquidLevelSensor) == LOW) {
		//		Pext.digitalWrite(LiquidLevelLed, HIGH);
		//	} else {
		//		Pext.digitalWrite(LiquidLevelLed, LOW);
		//	}
	}

	uint8_t Fireplace::getFireplaceAcLightRelay() const
	{
		return fireplaceAcLightRelay;
	}

	void Fireplace::setFireplaceAcLightRelay(uint8_t fireplaceAcLightRelay)
	{
		this->fireplaceAcLightRelay = fireplaceAcLightRelay;
	}

	uint8_t Fireplace::getFireplaceButtont() const
	{
		return fireplaceButtont;
	}

	void Fireplace::setFireplaceButtont(uint8_t fireplaceButtont)
	{
		this->fireplaceButtont = fireplaceButtont;
	}

	uint8_t Fireplace::getFireplaceCoolerOut() const
	{
		return fireplaceCoolerOut;
	}

	void Fireplace::setFireplaceCoolerOut(uint8_t fireplaceCoolerOut)
	{
		this->fireplaceCoolerOut = fireplaceCoolerOut;
	}

	uint8_t Fireplace::getFireplaceLedOut() const
	{
		return fireplaceLedOut;
	}

	void Fireplace::setFireplaceLedOut(uint8_t fireplaceLedOut)
	{
		this->fireplaceLedOut = fireplaceLedOut;
	}

	uint8_t Fireplace::getFireplaceSmokeRelay() const
	{
		return fireplaceSmokeRelay;
	}

	void Fireplace::setFireplaceSmokeRelay(uint8_t fireplaceSmokeRelay)
	{
		this->fireplaceSmokeRelay = fireplaceSmokeRelay;
	}

	DFRobotDFPlayerMini *Fireplace::getPlayer() const
	{
		return player;
	}

	uint8_t Fireplace::getLiquidLevelLed() const
	{
		return LiquidLevelLed;
	}

	void Fireplace::setLiquidLevelLed(uint8_t liquidLevelLed)
	{
		LiquidLevelLed = liquidLevelLed;
	}

	uint8_t Fireplace::getLiquidLevelSensor() const
	{
		return liquidLevelSensor;
	}

	void Fireplace::setLiquidLevelSensor(uint8_t liquidLevelSensor)
	{
		this->liquidLevelSensor = liquidLevelSensor;
	}

	void Fireplace::setPlayer(DFRobotDFPlayerMini *player)
	{
		this->player = player;
	}

	Fireplace::~Fireplace()
	{
		// TODO Auto-generated destructor stub
	}

} /* namespace SEFL */

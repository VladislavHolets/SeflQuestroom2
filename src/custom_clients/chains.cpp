/*
 * chains.cpp
 *
 *  Created on: Jan 31, 2022
 *      Author: piatk
 */

#include <custom_clients/chains.h>
#include <utils/logger.h>
#include <peripherals/multpinsextender.h>
#include <peripherals/pwmpinsextender.h>
#include <wiring_constants.h>
#include <wiring_digital.h>
#include <wiring_time.h>

uint8_t prevleftchain = 0;
uint8_t lcount = 0;

uint8_t prevrightchain = 0;
uint8_t rcount = 0;

namespace SEFL
{

	Chains::Chains(MQTTClient &mqtt, const char *name, uint8_t reset_status,
				   const char *placement, const char *in_topic, const char *out_topic,
				   SEFL::Language language) : Quest_Basic_Client(mqtt, name, reset_status, placement, in_topic,
																 out_topic, language)
	{

		chainsLeftButton = 0;
		chainsRightButton = 1;
		chainsLeftMagnet = 0;
		chainsRightMagnet = 1;
	}

	void Chains::onActive()
	{

		if (isChangedStatus())
		{
			unsetChangedStatus();
			SEFL::Logger::verbose("Chains", "onActive");

			this->reportStatus();

			Pext.digitalWrite(chainsLeftMagnet, true);
			Pext.digitalWrite(chainsRightMagnet, true);
			prevleftchain = 0;
			prevrightchain = 0;
			lcount = 0;
			rcount = 0;

			Mext.digitalRead(chainsLeftButton);
			pinMode(Mext.getCi(), INPUT_PULLUP);
		}

		//	Mext.digitalRead(chainsLeftButton);
		//	delay(100);
		//	SEFL::Logger::verbose("ChainsLeft", Mext.digitalRead(chainsLeftButton));
		//
		//	Mext.digitalRead(chainsRightButton);
		//	delay(100);
		//	SEFL::Logger::verbose("ChainsRight            ", Mext.digitalRead(chainsRightButton));

		if (prevleftchain != Mext.digitalRead(chainsLeftButton))
		{
			lcount++;
			SEFL::Logger::verbose("ChainsLeft", lcount);
		}
		if (prevrightchain != Mext.digitalRead(chainsRightButton))
		{
			rcount++;
			SEFL::Logger::verbose("ChainsRight", rcount);
		}

		prevleftchain = Mext.digitalRead(chainsLeftButton);
		prevrightchain = Mext.digitalRead(chainsRightButton);

		if (lcount > 2 && rcount > 2)
		{
			this->setStatus(BasicClientStatuses::FINISHED_STATUS);
		}
	}

	void Chains::onFinished()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			SEFL::Logger::verbose("Chains", "onFinished");
			this->reportStatus();
		}
	}
	void Chains::onManualFinished()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("Chains", "onManualFinished");
		}
	}

	void Chains::onDefault()
	{
		if (isChangedStatus())
		{
			this->reportStatus();
			unsetChangedStatus();
			SEFL::Logger::verbose("Chains", "onDefault");
			Pext.digitalWrite(chainsLeftMagnet, false);
			Pext.digitalWrite(chainsRightMagnet, false);
		}
	}

	uint8_t Chains::getChainsLeftButton() const
	{
		return chainsLeftButton;
	}

	void Chains::setChainsLeftButton(uint8_t chainsLeftButton)
	{
		this->chainsLeftButton = chainsLeftButton;
	}

	uint8_t Chains::getChainsLeftMagnet() const
	{
		return chainsLeftMagnet;
	}

	void Chains::setChainsLeftMagnet(uint8_t chainsLeftMagnet)
	{
		this->chainsLeftMagnet = chainsLeftMagnet;
	}

	uint8_t Chains::getChainsRightButton() const
	{
		return chainsRightButton;
	}

	void Chains::setChainsRightButton(uint8_t chainsRightButton)
	{
		this->chainsRightButton = chainsRightButton;
	}

	uint8_t Chains::getChainsRightMagnet() const
	{
		return chainsRightMagnet;
	}

	void Chains::setChainsRightMagnet(uint8_t chainsRightMagnet)
	{
		this->chainsRightMagnet = chainsRightMagnet;
	}

	Chains::~Chains()
	{
		// TODO Auto-generated destructor stub
	}

} /* namespace SEFL */

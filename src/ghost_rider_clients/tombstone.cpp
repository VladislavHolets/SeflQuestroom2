/*
 * tombstone.cpp
 *
 *  Created on: Jan 25, 2022
 *      Author: piatk
 */

#include <sys/_stdint.h>
#include "tombstone.h"
#include <wiring_constants.h>
#include <wiring_digital.h>

namespace SEFL
{

	Tombstone::Tombstone(MQTTClient &mqtt, const char *name, uint8_t reset_status,
						 const char *placement, const char *in_topic, const char *out_topic,
						 SEFL::Language language) : Quest_Basic_Client(mqtt, name, reset_status, placement, in_topic,
																	   out_topic, language)
	{
		magnet_tombstone = 0;
		inverted = 0;
		motojacket_button = 1;
	}

	Tombstone::~Tombstone()
	{
		// TODO Auto-generated destructor stub
	}

	void Tombstone::onActive()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			Mext.digitalRead(motojacket_button);
			pinMode(Mext.getCi(), INPUT_PULLUP);
		}
		Pext.digitalWrite(magnet_tombstone, !inverted);
		SEFL::Logger::verbose("Tombstone", Mext.digitalRead(motojacket_button));

		if (Mext.digitalRead(motojacket_button) == LOW)
		{
			this->setStatus(BasicClientStatuses::FINISHED_STATUS);
		}
	}

	void Tombstone::onFinished()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
		}
	}

	void Tombstone::onManualFinished()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
		}
	}

	bool Tombstone::isInverted()
	{
		return inverted;
	}

	void Tombstone::setInverted(bool inverted)
	{
		this->inverted = inverted;
	}

	uint8_t Tombstone::getMagnetTombstone()
	{
		return magnet_tombstone;
	}

	uint8_t Tombstone::getMotojacketButton()
	{
		return motojacket_button;
	}

	void Tombstone::setMotojacketButton(uint8_t motojacketButton)
	{
		motojacket_button = motojacketButton;
	}

	void Tombstone::setMagnetTombstone(uint8_t magnetTombstone)
	{
		magnet_tombstone = magnetTombstone;
	}

	void Tombstone::onDefault()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("Tombstone", "onDefault");
		}

		Pext.digitalWrite(magnet_tombstone, inverted);
	}

} /* namespace SEFL */

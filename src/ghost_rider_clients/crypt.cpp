/*
 * crypt.cpp
 *
 *  Created on: Jan 21, 2022
 *      Author: piatk
 */

#include "crypt.h"
#include <sys/_stdint.h>
#include <wiring_constants.h>
#include <wiring_digital.h>

namespace SEFL
{
	bool leftTorchFlag;
	bool rightTorchFlag;
	Crypt::Crypt(MQTTClient &mqtt, const char *name, uint8_t reset_status,
				 const char *placement, const char *in_topic, const char *out_topic,
				 SEFL::Language language) : Quest_Basic_Client(mqtt, name, reset_status, placement, in_topic,
															   out_topic, language)
	{
		leftRelay = 0;
		rightRelay = 1;
		leftTorch = 0;
		rightTorch = 1;
	}

	uint8_t Crypt::getLeftRelay()
	{
		return leftRelay;
	}

	void Crypt::setLeftRelay(uint8_t leftRelay)
	{
		this->leftRelay = leftRelay;
	}

	uint8_t Crypt::getLeftTorch()
	{
		return leftTorch;
	}

	void Crypt::setLeftTorch(uint8_t leftTorch)
	{
		this->leftTorch = leftTorch;
	}

	uint8_t Crypt::getRightRelay()
	{
		return rightRelay;
	}

	void Crypt::setRightRelay(uint8_t rightRelay)
	{
		this->rightRelay = rightRelay;
	}

	uint8_t Crypt::getRightTorch()
	{
		return rightTorch;
	}

	void Crypt::setRightTorch(uint8_t rightTorch)
	{
		this->rightTorch = rightTorch;
	}

	Crypt::~Crypt()
	{
		// TODO Auto-generated destructor stub
	}

	void Crypt::onActive()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();

			leftTorchFlag = false;
			rightTorchFlag = false;
			SEFL::Logger::verbose("Crypt", "OnActive");
		}

		Mext.digitalRead(leftTorch);
		pinMode(Mext.getCi(), INPUT_PULLUP);

		//	SEFL::Logger::verbose("Crypt","Torch buttons");
		//	SEFL::Logger::verbose("Crypt","leftTorch");
		//	SEFL::Logger::verbose("Crypt",Mext.digitalRead(leftTorch));
		//	SEFL::Logger::verbose("Crypt","rightTorch");
		//	SEFL::Logger::verbose("Crypt",Mext.digitalRead(rightTorch));

		if (Mext.digitalRead(leftTorch) == LOW && leftTorchFlag == false)
		{
			Pext.digitalWrite(leftTorch, LOW);
			leftTorchFlag = true;
		}
		if (Mext.digitalRead(rightTorch) == LOW && rightTorchFlag == false)
		{
			Pext.digitalWrite(rightTorch, LOW);
			rightTorchFlag = true;
		}
		if (leftTorchFlag && rightTorchFlag)
		{
			this->setStatus(BasicClientStatuses::FINISHED_STATUS);
		}

		//	Pext.digitalWrite(leftRelay, !Mext.digitalRead(leftTorch));
		//	Pext.digitalWrite(rightRelay, !Mext.digitalRead(rightTorch));

		//  bool lt = Mext.digitalRead(leftTorch);
		//  bool rt = Mext.digitalRead(rightTorch);
		//  if (lt && rt) {
		//		this->setStatus(BasicClientStatuses::FINISHED_STATUS);
		//	}

		//		if (!Mext.digitalRead(leftTorch) && !Mext.digitalRead(rightTorch)) {
		//			this->setStatus(BasicClientStatuses::FINISHED_STATUS);
		//		}
	}
	void Crypt::onFinished()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			Pext.digitalWrite(leftRelay, false);
			Pext.digitalWrite(rightRelay, false);
			SEFL::Logger::verbose("Crypt", "OnFinished");
		}
	}

	void Crypt::onManualFinished()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			Pext.digitalWrite(leftRelay, false);
			Pext.digitalWrite(rightRelay, false);
			SEFL::Logger::verbose("Crypt", "onManualFinished");
		}
	}

	void Crypt::onDefault()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			Pext.digitalWrite(leftTorch, true);
			Pext.digitalWrite(rightTorch, true);
			SEFL::Logger::verbose("Crypt", "OnDefault");
		}
	}

} /* namespace SEFL */

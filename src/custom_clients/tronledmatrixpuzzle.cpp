/*
 * tronledmatrixpuzzle.cpp
 *
 *  Created on: Jun 8, 2021
 *      Author: vladi
 */

#include <peripherals/pwmpinsextender.h>
#include <peripherals/multpinsextender.h>
#include <custom_clients/tronledmatrixpuzzle.h>

namespace SEFL
{

	TronLEDMatrixPuzzle::~TronLEDMatrixPuzzle()
	{
		// TODO Auto-generated destructor stub
	}

	void TronLEDMatrixPuzzle::onActive()
	{
		if (this->changed_status_)
		{
			this->changed_status_ = false;
			this->reportStatus();
			pinMode(Mext.getCi(), INPUT_PULLDOWN);
		}
		for (int i = 0; i < 9; i++)
		{
			if (Mext.digitalRead(i) == HIGH)
			{
				int temp = nextColor(i);
				this->current[i][0] = constant_array[temp].r;
				this->current[i][1] = constant_array[temp].g;
				this->current[i][2] = constant_array[temp].b;
			}
		}
		bool solved = true;
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (this->current[i][j] != true)
				{
					solved = false;
				}
			}
		}
		if (solved == true)
		{
			this->setStatus(SEFL::BasicClientStatuses::FINISHED_STATUS);
		}
		delay(100);
	}

	void TronLEDMatrixPuzzle::onFinished()
	{
		if (this->changed_status_)
		{
			this->changed_status_ = false;
			this->reportStatus();
			pinMode(Mext.getCi(), INPUT_PULLDOWN);
		}
	}

	void TronLEDMatrixPuzzle::onManualFinished()
	{
		if (this->changed_status_)
		{
			this->changed_status_ = false;
			this->reportStatus();
			pinMode(Mext.getCi(), INPUT_PULLDOWN);
		}
	}

	const uint8_t *TronLEDMatrixPuzzle::getAnodes()
	{
		return anodes;
	}

	void TronLEDMatrixPuzzle::setAnodes(const uint8_t *anodes_)
	{
	}
	const uint8_t *TronLEDMatrixPuzzle::getCathodes()
	{
		return cathodes;
	}

	TronLEDMatrixPuzzle::TronLEDMatrixPuzzle(MQTTClient &mqtt, const char *name,
											 uint8_t reset_status, const char *placement, const char *in_topic,
											 const char *out_topic, SEFL::Language language) : Quest_Basic_Client(mqtt, name, reset_status, placement, in_topic,
																												  out_topic, language),
																							   anodes{0, 1, 2, 3, 4, 5, 6, 7, 8}, cathodes{
																																	  9, 10, 11},
																							   anodes_amount(9), cathodes_amount(3), current{0}
	{
	}

	uint8_t TronLEDMatrixPuzzle::nextColor(uint8_t color)
	{
		for (int i = 0; i < 5; i++)
		{
			if (current[color][0] == constant_array[i].r && current[color][1] == constant_array[i].g && current[color][2] == constant_array[i].b)
			{
				return (((i < 4) ? i + 1 : 0));
			}
		}
		return (0);
	}

	uint8_t TronLEDMatrixPuzzle::getAnodesAmount() const
	{
		return anodes_amount;
	}

	uint8_t TronLEDMatrixPuzzle::getCathodesAmount() const
	{
		return cathodes_amount;
	}

	uint8_t *TronLEDMatrixPuzzle::getCurrent()
	{
		return &current[0][0];
	}

	void TronLEDMatrixPuzzle::setCathodes(const uint8_t *cathodes_)
	{
	}

	void TronLEDMatrixPuzzle::onDefault()
	{
		if (this->changed_status_)
		{
			this->changed_status_ = false;
			this->reportStatus();
			for (int i = 0; i < anodes_amount; ++i)
			{
				for (int j = 0; j < cathodes_amount; ++j)
				{
					this->current[i][j] = 0;
				}
			}
		}
	}

} /* namespace SEFL */

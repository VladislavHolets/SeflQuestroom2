/*
 * tronledmatrixpuzzleneopixel.cpp
 *
 *  Created on: Oct 16, 2022
 *      Author: vladi
 */
#include "tronledmatrixpuzzleneopixel.h"

namespace SEFL
{

	TronLEDMatrixPuzzleNeopixel::~TronLEDMatrixPuzzleNeopixel()
	{
		// TODO Auto-generated destructor stub
	}

	void TronLEDMatrixPuzzleNeopixel::onDefault()
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

	void TronLEDMatrixPuzzleNeopixel::onActive()
	{
		if (this->changed_status_)
		{
			this->changed_status_ = false;
			this->reportStatus();
			pinMode(Mext.getCi(), INPUT_PULLUP);
			this->initButtons();
		}
		this->scanButtons();
		bool solved = checkPattern();
		if (solved == true)
		{
			this->setStatus(SEFL::BasicClientStatuses::FINISHED_STATUS);
		}
		delay(100);
	}

	void TronLEDMatrixPuzzleNeopixel::onFinished()
	{
		if (this->changed_status_)
		{
			this->changed_status_ = false;
			this->reportStatus();
			pinMode(Mext.getCi(), INPUT_PULLDOWN);
		}
	}

	void TronLEDMatrixPuzzleNeopixel::onManualFinished()
	{
		if (this->changed_status_)
		{
			this->changed_status_ = false;
			this->reportStatus();
			pinMode(Mext.getCi(), INPUT_PULLDOWN);
		}
	}

	TronLEDMatrixPuzzleNeopixel::TronLEDMatrixPuzzleNeopixel(MQTTClient &mqtt, const char *name,
															 uint8_t reset_status, const char *placement, const char *in_topic,
															 const char *out_topic, SEFL::Language language) : Quest_Basic_Client(mqtt, name, reset_status, placement, in_topic,
																																  out_topic, language)
	{
	}
	void TronLEDMatrixPuzzleNeopixel::initButtons()
	{
		for (int i = 0; i < this->buttons_amount; i++)
		{
			this->buttons_states[i] = ButtonState::RESETED;
			this->buttons_colors[i] = 0;
		}
	}

	void TronLEDMatrixPuzzleNeopixel::scanButtons()
	{
		bool refreshFlag = false;
		for (int i = 0; i < this->buttons_amount; i++)
		{
			bool value = Mext.digitalRead(this->buttons_pins[i]);
			if (value == LOW && this->buttons_states[i] == ButtonState::RESETED)
			{
				this->buttons_states[i] = ButtonState::PRESSED;
			}
			else if (value == HIGH && this->buttons_states[i] == ButtonState::PRESSED)
			{
				this->buttons_states[i] = ButtonState::RELEASED;
				this->buttons_colors[i] = (this->buttons_colors[i] < (colors_size - 1)) ? this->buttons_colors[i] + 1 : 0;
				refreshFlag = true;
				this->buttons_states[i] = ButtonState::RESETED;
			}
		}
		if (refreshFlag)
		{
			this->refreshStrip();
		}
	}
	void TronLEDMatrixPuzzleNeopixel::refreshStrip()
	{
		Mext.digitalRead(this->strip_pin);
		for (int i = 0; i < this->buttons_amount; i++)
		{
			this->strip.fill(this->getColor(this->buttons_colors[i]), i * segment_size, segment_size);
		}
		this->strip.show();
	}
	bool TronLEDMatrixPuzzleNeopixel::checkPattern()
	{
		for (size_t i = 0; i < this->buttons_amount; i++)
		{
			if (this->buttons_colors != this->buttons_correct_colors)
			{
				return false;
			}
		}
		return true;
	}
	void TronLEDMatrixPuzzleNeopixel::setPattern(uint8_t *array, uint8_t size)
	{
		if (this->buttons_amount == size)
		{
			for (int i = 0; i < this->buttons_amount; i++)
			{
				this->buttons_correct_colors[i] = array[size];
			}
		}
	}
	void TronLEDMatrixPuzzleNeopixel::setButtons(uint8_t *array, uint8_t size)
	{
		this->buttons_amount = size;
		if (this->buttons_pins != nullptr)
		{
			delete this->buttons_pins;
			delete this->buttons_states;
			delete this->buttons_colors;
			delete this->buttons_correct_colors;
		}
		this->buttons_pins = new uint8_t[size];
		this->buttons_states = new uint8_t[size];
		this->buttons_colors = new uint8_t[size];
		this->buttons_correct_colors = new uint8_t[size];
		this->strip.updateLength(this->buttons_amount * segment_size);
		for (int i = 0; i < this->buttons_amount; i++)
		{
			this->buttons_pins[i] = array[size];
		}
		this->initButtons();
	}
	void setPattern(uint8_t *array, uint8_t size);
	uint8_t *TronLEDMatrixPuzzleNeopixel::getButtons()
	{
		return this->buttons_pins;
	}
	uint8_t TronLEDMatrixPuzzleNeopixel::getButtonsSize()
	{
		return this->buttons_amount;
	}
	void TronLEDMatrixPuzzleNeopixel::setStripSegmentSize(uint8_t segment_size)
	{
		if (segment_size != 0)
		{
			this->segment_size = segment_size;
		}
	}
	uint8_t TronLEDMatrixPuzzleNeopixel::getColor(uint8_t color)
	{

		static const uint32_t colors[colors_size]{
			Adafruit_NeoPixel::Color(0, 0, 0),
			Adafruit_NeoPixel::Color(255, 0, 0),
			Adafruit_NeoPixel::Color(0, 255, 0),
			Adafruit_NeoPixel::Color(255, 255, 0),
			Adafruit_NeoPixel::Color(0, 0, 255),
			Adafruit_NeoPixel::Color(255, 0, 255),
			Adafruit_NeoPixel::Color(0, 255, 255),
			Adafruit_NeoPixel::Color(255, 255, 255),
		};
	}

} /* namespace SEFL */

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
			initButtons();
			refreshStrip();
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
		if (solved)
		{
			this->setStatus(SEFL::BasicClientStatuses::FINISHED_STATUS);
		}
		// delay(100);
	}

	void TronLEDMatrixPuzzleNeopixel::onFinished()
	{
		if (this->changed_status_)
		{
			this->changed_status_ = false;
			this->reportStatus();
			pinMode(Mext.getCi(), INPUT_PULLUP);
		}
	}

	void TronLEDMatrixPuzzleNeopixel::onManualFinished()
	{
		if (this->changed_status_)
		{
			this->changed_status_ = false;
			this->reportStatus();
			pinMode(Mext.getCi(), INPUT_PULLUP);
		}
	}

	TronLEDMatrixPuzzleNeopixel::TronLEDMatrixPuzzleNeopixel(MQTTClient &mqtt, const char *name,
															 uint8_t reset_status, const char *placement, const char *in_topic,
															 const char *out_topic, SEFL::Language language) : Quest_Basic_Client(mqtt, name, reset_status, placement, in_topic,
																																  out_topic, language)
	{
        this->buttons_colors= nullptr;
        this->buttons_correct_colors= nullptr;
        this->buttons_pins= nullptr;
        this->buttons_states= nullptr;
        this->buttons_amount=0;
        this->strip.setPin(Mext.getCi());
        this->strip.updateType(NEO_GRB + NEO_KHZ800);
        Mext.digitalRead(this->strip_pin);
        pinMode(Mext.getCi(),INPUT);
        strip.begin();
        strip.clear();
        this->strip.show();
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
       // SEFL::Logger::notice(this->getName(), "scanning buttons");
        pinMode(Mext.getCi(), INPUT_PULLUP);
		bool refreshFlag = false;
		for (int i = 0; i < this->buttons_amount; i++)
		{
			bool value = Mext.digitalRead(this->buttons_pins[i]);

           // SEFL::Logger::notice(this->getName(), buttons_pins[i]);
           // SEFL::Logger::notice(this->getName(), value);
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
            SEFL::Logger::notice(this->getName(), "LED REFRESH TIME");
			this->refreshStrip();
		}
        //this->refreshStrip();
	}
	void TronLEDMatrixPuzzleNeopixel::refreshStrip()
	{
		Mext.digitalRead(this->strip_pin);
        pinMode(Mext.getCi(),INPUT);
		strip.begin();
        //strip.clear();
       // this->strip.show();
		for (int i = 0; i < this->buttons_amount; i++)
		{
			//this->strip.fill(this->getColor(this->buttons_colors[i]), i * segment_size, segment_size);
		for (int j=0;j<segment_size;j++){
            this->strip.setPixelColor(i*segment_size+j,this->getColor(this->buttons_colors[i]));
        }
        }
        this->strip.show();
		this->strip.show();
	}
	bool TronLEDMatrixPuzzleNeopixel::checkPattern()
	{
		for (size_t i = 0; i < this->buttons_amount; i++)
		{
			if (this->buttons_colors[i] != this->buttons_correct_colors[i])
			{
				return false;
			}
		}
		return true;
	}
	void TronLEDMatrixPuzzleNeopixel::setStripPin(uint8_t pin)
	{
		if (pin >= 0 && pin < 16)
		{
			this->strip_pin = pin;
		}
	}
	void TronLEDMatrixPuzzleNeopixel::setPattern(const uint8_t *array, uint8_t size)
	{
		if (this->buttons_amount == size)
		{
			for (int i = 0; i < this->buttons_amount; i++)
			{
				this->buttons_correct_colors[i] = array[i];
			}
		}
	}
	void TronLEDMatrixPuzzleNeopixel::setButtons(const uint8_t *array, uint8_t size)
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
			this->buttons_pins[i] = array[i];
		}
		this->initButtons();
	}
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

        this->strip.updateLength(this->buttons_amount * segment_size);
	}
	uint32_t TronLEDMatrixPuzzleNeopixel::getColor(uint8_t color)
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
		return (colors[color % colors_size]);
	}

} /* namespace SEFL */

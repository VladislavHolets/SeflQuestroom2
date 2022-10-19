/*
 * tronledmatrixpuzzle.h
 *
 *  Created on: Jun 8, 2021
 *      Author: vladi
 */

#ifndef SEFL_TRONLEDMATRIXPUZZLE_H_
#define SEFL_TRONLEDMATRIXPUZZLE_H_
#include <Adafruit_NeoPixel.h>
#include <sefl_quest_core.h>
#include <sys/_stdint.h>

namespace SEFL
{

	class TronLEDMatrixPuzzleNeopixel : public Quest_Basic_Client
	{
		static const uint8_t colors_size = 8;

		uint8_t buttons_amount;
		uint8_t *buttons_pins;
		uint8_t *buttons_states;
		uint8_t *buttons_colors;
		uint8_t *buttons_correct_colors;
		Adafruit_NeoPixel strip;
		uint8_t segment_size;
		uint8_t strip_pin;
		enum ButtonState
		{
			RESETED,
			PRESSED,
			RELEASED
		};
		void scanButtons();
		void initButtons();
		void refreshStrip();
		bool checkPattern();

	public:
		explicit TronLEDMatrixPuzzleNeopixel(MQTTClient &mqtt, const char *name = DEFAULT_DEVICE_NAME,
									uint8_t reset_status = 1, const char *placement = DEFAULT_PLACEMENT,
									const char *in_topic = DEFAUT_IN_TOPIC, const char *out_topic = DEFAUT_OUT_TOPIC, SEFL::Language language = UKR);
		~TronLEDMatrixPuzzleNeopixel() override;
		// user defined
		void setStripPin(uint8_t pin);
		void setButtons(const uint8_t *array, uint8_t size);
		void setPattern(const uint8_t *array, uint8_t size);
		uint8_t *getButtons();
		uint8_t getButtonsSize();
		void setStripSegmentSize(uint8_t segment_size);
		uint32_t getColor(uint8_t color);

		// predefined for the device type
		void onActive() override;
		void onFinished() override;
		void onManualFinished() override;
		void onDefault() override;


    };

} /* namespace SEFL */

#endif /* SEFL_TRONLEDMATRIXPUZZLE_H_ */

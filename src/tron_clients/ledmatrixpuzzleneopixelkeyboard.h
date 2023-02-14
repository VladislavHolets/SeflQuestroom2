//
// Created by vladi on 18.10.2022.
//

#ifndef SEFLQUESTROOM2_LEDMATRIXPUZZLENEOPIXELKEYBOARD_H
#define SEFLQUESTROOM2_LEDMATRIXPUZZLENEOPIXELKEYBOARD_H


#include "Adafruit_NeoPixel.h"
#include "sefl_quest_core.h"

namespace SEFL {

    class LEDMatrixPuzzleNeopixelKeyboard: public Quest_Basic_Client {
    private :
        PextMextKeyboard keyboard;

        static const uint8_t colors_size = 8;

        int8_t buttons_amount;
        uint8_t *buttons_states;
        uint8_t *buttons_colors;
        uint8_t *buttons_correct_colors;
        Adafruit_NeoPixel strip;
        int8_t segment_size;
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
        LEDMatrixPuzzleNeopixelKeyboard(MQTTClient &mqtt, const char *name, uint8_t resetStatus,
                                        const char *placement, const char *inTopic=DEFAUT_IN_TOPIC, const char *outTopic=DEFAUT_OUT_TOPIC,
                                        Language language=ENG);
        void setKeyboardPextPins(const uint8_t * pextPins,uint8_t pextPinsSize);
        void setKeyboardMextPins(const uint8_t * mextPins,uint8_t mextPinsSize);
        void setKeyboardKeymap(const char *keyMap, uint8_t keymapRowSize, uint8_t keymapCollSize);
        // user defined
        void setStripPin(uint8_t pin);
        void setPattern(const uint8_t *array, int8_t size);
        uint8_t getButtonsSize() const;
        void setStripSegmentSize(int8_t segment_size);
        static uint32_t getColor(uint8_t color);

        // predefined for the device type
        void onActive() override;
        void onFinished() override;
        void onManualFinished() override;
        void onDefault() override;


    };

} // SEFL

#endif //SEFLQUESTROOM2_LEDMATRIXPUZZLENEOPIXELKEYBOARD_H

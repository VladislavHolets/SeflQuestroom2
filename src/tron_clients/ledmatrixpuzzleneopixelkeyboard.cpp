//
// Created by vladi on 18.10.2022.
//

#include "ledmatrixpuzzleneopixelkeyboard.h"

namespace SEFL {
    LEDMatrixPuzzleNeopixelKeyboard::LEDMatrixPuzzleNeopixelKeyboard(MQTTClient &mqtt, const char *name,
                                                                     uint8_t resetStatus, const char *placement,
                                                                     const char *inTopic, const char *outTopic,
                                                                     Language language) : Quest_Basic_Client(
            mqtt, name, resetStatus, placement, inTopic, outTopic, language) {
        this->buttons_colors= nullptr;
        this->buttons_correct_colors= nullptr;
        this->buttons_states= nullptr;
        this->buttons_amount=0;
        this->strip.setPin(Mext.getCi());
        this->strip.updateType(NEO_GRB + NEO_KHZ800);
//        Mext.digitalRead(this->strip_pin);
//        pinMode(Mext.getCi(),INPUT);
//        strip.begin();
//        strip.clear();
//        this->strip.show();
    }

    void LEDMatrixPuzzleNeopixelKeyboard::onDefault()
    {
        if (this->changed_status_)
        {
            this->changed_status_ = false;
            this->reportStatus();
            //initButtons();
            //refreshStrip();
        }
    }

    void LEDMatrixPuzzleNeopixelKeyboard::onActive()
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

    void LEDMatrixPuzzleNeopixelKeyboard::onFinished()
    {
        if (this->changed_status_)
        {
            this->changed_status_ = false;
            this->reportStatus();
            initButtons();
            refreshStrip();
            pinMode(Mext.getCi(), INPUT_PULLUP);
        }
    }

    void LEDMatrixPuzzleNeopixelKeyboard::onManualFinished()
    {
        onFinished();
    }

    void LEDMatrixPuzzleNeopixelKeyboard::setKeyboardPextPins(const uint8_t *pextPins, uint8_t pextPinsSize) {
        this->keyboard.setPextPins(pextPins,pextPinsSize);
    }

    void LEDMatrixPuzzleNeopixelKeyboard::setKeyboardMextPins(const uint8_t *mextPins, uint8_t mextPinsSize) {
        this->keyboard.setMextPins(mextPins,mextPinsSize);
    }

    void LEDMatrixPuzzleNeopixelKeyboard::setKeyboardKeymap(const char *keyMap, uint8_t keymapRowSize, uint8_t keymapCollSize) {
        this->keyboard.setKeymap(keyMap,keymapRowSize,keymapCollSize);
        this->buttons_amount = keymapRowSize*keymapCollSize;
        if (this->buttons_states != nullptr)
        {
            delete this->buttons_states;
            delete this->buttons_colors;
            delete this->buttons_correct_colors;
        }
        this->buttons_states = new uint8_t[buttons_amount];
        this->buttons_colors = new uint8_t[buttons_amount];
        this->buttons_correct_colors = new uint8_t[buttons_amount];
        this->strip.updateLength(this->buttons_amount * segment_size);
        this->initButtons();
    }

    void LEDMatrixPuzzleNeopixelKeyboard::setStripPin(uint8_t pin) {
        if (pin >= 0 && pin < 16)
        {
            this->strip_pin = pin;
        }
    }


    void LEDMatrixPuzzleNeopixelKeyboard::setPattern(const uint8_t *array, uint8_t size) {
        if (this->buttons_amount == size)
        {
            for (int i = 0; i < this->buttons_amount; i++)
            {
                this->buttons_correct_colors[i] = array[i];
            }
        }
    }

    void LEDMatrixPuzzleNeopixelKeyboard::setStripSegmentSize(uint8_t segment_size) {
        if (segment_size != 0)
        {
            this->segment_size = segment_size;
        }

        this->strip.updateLength(this->buttons_amount * segment_size);
    }

    uint32_t LEDMatrixPuzzleNeopixelKeyboard::getColor(uint8_t color) {
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



    void LEDMatrixPuzzleNeopixelKeyboard::scanButtons() {
// SEFL::Logger::notice(this->getName(), "scanning buttons");
        pinMode(Mext.getCi(), INPUT_PULLUP);
        bool refreshFlag = false;
        static char value[2]={' ','\0'};
        value[0]=keyboard.getKey();

        //Logger::notice(this->getName(),value);
        if(value[0]!=0) {
            //Logger::notice(this->getName(),value[0]);
            int i = value[0]-'1';
            if (this->buttons_states[i] == ButtonState::RESETED) {
                this->buttons_states[i] = ButtonState::PRESSED;
                this->buttons_colors[i] = (this->buttons_colors[i] < (colors_size - 1)) ? this->buttons_colors[i] + 1
                                                                                        : 0;
                refreshFlag = true;
                this->buttons_states[i] = ButtonState::RESETED;
            }
        }
        if (refreshFlag)
        {
            //SEFL::Logger::notice(this->getName(), "LED REFRESH BEGIN");
            this->refreshStrip();

            //SEFL::Logger::notice(this->getName(), "LED REFRESH END");
        }
        //this->refreshStrip();
    }

    void LEDMatrixPuzzleNeopixelKeyboard::initButtons() {

        for (int i = 0; i < this->buttons_amount; i++)
        {
            this->buttons_states[i] = ButtonState::RESETED;
            this->buttons_colors[i] = 0;
        }

    }

    void LEDMatrixPuzzleNeopixelKeyboard::refreshStrip() {
        Mext.digitalRead(this->strip_pin);
        pinMode(Mext.getCi(),INPUT);
        //Logger::notice(this->getName(),"BEFORE STRIP BEGIN");
        strip.begin();

        //Logger::notice(this->getName(),"AFTER STRIP BEGIN");
        //strip.clear();
        // this->strip.show();
        for (int i = 0; i < this->buttons_amount; i++)
        {
            this->strip.fill(this->getColor(this->buttons_colors[i]), i * segment_size, segment_size);
//           for (int j=0;j<segment_size;j++){
//                this->strip.setPixelColor(i*segment_size+j,this->getColor(this->buttons_colors[i]));
//            }
        }
        this->strip.show();
        this->strip.show();
    }

    bool LEDMatrixPuzzleNeopixelKeyboard::checkPattern() {
        for (size_t i = 0; i < this->buttons_amount; i++)
        {
            if (this->buttons_colors[i] != this->buttons_correct_colors[i])
            {
                return false;
            }
        }
        return true;
    }

    uint8_t LEDMatrixPuzzleNeopixelKeyboard::getButtonsSize() {
        return this->buttons_amount;
    }

} // SEFL
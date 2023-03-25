//
// Created by vladi on 18.02.2023.
//

#include "laserbarier.h"

namespace SEFL {
    LaserBarier::LaserBarier(MQTTClient &mqtt, const char *name, uint8_t resetStatus, const char *placement,
                             const char *inTopic, const char *outTopic, Language language) : Quest_Basic_Client(mqtt,
                                                                                                                name,
                                                                                                                resetStatus,
                                                                                                                placement,
                                                                                                                inTopic,
                                                                                                                outTopic,
                                                                                                                language) {
        segments_pins = nullptr;
        segments_size = 0;

        barrier = nullptr;
        lasers_pin = 0;
        puzzle_state = INITIAL;
        curr_cross = 0;

        incorrect_animation_timeout=5000;
        correct_animation_timeout=5000;
        animation_timestamp=0;

    }

    void LaserBarier::onActive() {
        if (this->changed_status_)
        {
            this->changed_status_ = false;
            this->reportStatus();
            Pext.digitalWrite(lasers_pin, LOW);
        }
        refresh_puzzle();
    }

    void LaserBarier::onDefault() {
        if (this->changed_status_)
        {
            this->changed_status_ = false;
            this->reportStatus();
        }
        init();
    }

    void LaserBarier::onFinished() {
        if (this->changed_status_)
        {
            this->changed_status_ = false;
            this->reportStatus();
        }
        Pext.digitalWrite(color_pins.green, HIGH);
        for(int i = 0; i < segments_size; i++) {
            Pext.digitalWrite(segments_pins[i], LOW);
        }
        Pext.digitalWrite(code_sign_pin, HIGH);
    }

    void LaserBarier::onManualFinished() {
        if (this->changed_status_)
        {
            this->changed_status_ = false;
            this->reportStatus();
            animation_timestamp = millis();
        }
        show_success();
        Pext.digitalWrite(color_pins.green, HIGH);
        for(int i = 0; i < segments_size; i++) {
            Pext.digitalWrite(segments_pins[i], HIGH);
        }
        Pext.digitalWrite(code_sign_pin, HIGH);
    }

    void LaserBarier::init() {
        animation_timestamp = 0;
        curr_cross = 0;
        puzzle_state = INITIAL;

        //turn off the LED
        for(int i = 0; i < segments_size; i++) {
            Pext.digitalWrite(segments_pins[i], HIGH);
        }

        //reset color
        Pext.digitalWrite(color_pins.red, LOW);
        Pext.digitalWrite(color_pins.green, LOW);
        Pext.digitalWrite(color_pins.blue, LOW);

        //code sign turn off
        Pext.digitalWrite(code_sign_pin, LOW);

        Pext.digitalWrite(lasers_pin, HIGH);

        //Mext pullup
        pinMode(Mext.getCi(),INPUT_PULLUP);
    }

    void LaserBarier::refresh_puzzle() {
        switch(puzzle_state) {
            case INITIAL: {
                read_barrier();
                break;
            }
            case CORRECT_ORDER: {
                show_success();
                if (animation_timestamp == 0) {
                    this->setStatus(FINISHED_STATUS);
                }
                break;
            }
            case INCORRECT_ORDER: {
                show_failure();
                if (animation_timestamp == 0) {
                    puzzle_state = INITIAL;
                }
                break;
            }
        }
    }


    void LaserBarier::read_barrier() {

        for(int i = 0; i < barrier_size; i++) {
            bool temp = Mext.digitalRead(barrier[i].sensor_pin);
            if (temp && barrier[i].crossed_order != curr_cross - 1) {
                Logger::notice("barrier", barrier[i].order_number);
                if (barrier[i].order_number != curr_cross)
                {
                    puzzle_state = INCORRECT_ORDER;
                    animation_timestamp = millis();
                    curr_cross = 0;
                    clearCurrentOrder();
                    return;
                }
                barrier[i].crossed_order = curr_cross;
                Pext.digitalWrite(segments_pins[curr_cross], LOW);
                Logger::notice("curr", curr_cross);
                curr_cross++;
            }
        }

        if (curr_cross == barrier_size) {
            puzzle_state = CORRECT_ORDER;
            animation_timestamp = millis();
            curr_cross = 0;
            clearCurrentOrder();
        }
    }


    void LaserBarier::show_success() {
        if (millis()-animation_timestamp > correct_animation_timeout) {
            animation_timestamp = 0;
            Pext.digitalWrite(color_pins.blue, LOW);
            return;
        }

        Pext.digitalWrite(lasers_pin, HIGH);

        auto state=(millis()-animation_timestamp)/(correct_animation_timeout/barrier_size);
        Pext.digitalWrite(color_pins.blue, HIGH);
        for(int i = 0; i < segments_size; i++) {
            Pext.digitalWrite(segments_pins[i], state!=i);
        }
    }

    void LaserBarier::show_failure() {
        if(millis()-animation_timestamp > incorrect_animation_timeout) {
            animation_timestamp = 0;
            Pext.digitalWrite(color_pins.red, LOW);
            Pext.digitalWrite(color_pins.blue, HIGH);
            for(int i = 0; i < segments_size; i++) {
                Pext.digitalWrite(segments_pins[i], HIGH);
            }
            return;
        }

        Pext.digitalWrite(color_pins.blue, LOW);
        Pext.digitalWrite(color_pins.red, HIGH);

        for(int i = 0; i < segments_size; i++) {
            Pext.digitalWrite(segments_pins[i], (millis()-animation_timestamp)/300%2);
        }

        Pext.digitalWrite(lasers_pin, (millis()-animation_timestamp)/300%2);
    }

    void LaserBarier::setIncorrectAnimationTimeout(uint32_t incorrectAnimationTimeout) {
        incorrect_animation_timeout = incorrectAnimationTimeout;
    }

    void LaserBarier::setCorrectAnimationTimeout(uint32_t correctAnimationTimeout) {
        correct_animation_timeout = correctAnimationTimeout;
    }

    void LaserBarier::setSegments(const uint8_t *segments, uint8_t segmentsSize) {
        segments_size = segmentsSize;

        if (segments_pins != nullptr) {
            delete [] segments_pins;
        }
        segments_pins = new uint8_t[segments_size];

        for(int i = 0; i < segments_size; i++) {
            segments_pins[i] = segments[i];
        }
    }

    void LaserBarier::setColor(ColorPins colorPins) {
        color_pins = colorPins;
    }

    void LaserBarier::setBarrier(const uint8_t *sensors, uint8_t lasersPin, uint8_t barrierSize) {
        if(barrier != nullptr) {
            delete [] barrier;
        }

        lasers_pin = lasersPin;
        barrier_size = barrierSize;
        barrier = new Barrier[barrier_size];

        for(int i = 0; i < barrier_size; i++) {
            barrier[i].sensor_pin = sensors[i];
            barrier[i].order_number = -1;
            barrier[i].crossed_order = -2;
        }
    }

    void LaserBarier::setOrder(const uint8_t *order, uint8_t orderSize) {
        if (orderSize != barrier_size || barrier == nullptr) {
            return;
        }

        for(int i = 0; i < barrier_size; i++) {
            barrier[i].order_number = order[i];
        }
    }

    void LaserBarier::clearCurrentOrder() {
        for(int i = 0; i < barrier_size; i++) {
            barrier[i].crossed_order = -2;
        }
    }

    void LaserBarier::setCodeSignPin(uint8_t codeSignPin) {
        code_sign_pin = codeSignPin;
    }

} // SEFL
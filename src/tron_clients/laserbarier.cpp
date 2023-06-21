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

        incorrect_animation_timeout = 5000;
        correct_animation_timeout = 5000;
        finished_puzzle_timeout = 60000;
        pause_timeout = 1000;
        animation_timestamp = 0;
        pause_timestamp = 0;

    }

    void LaserBarier::onActive() {
        if (this->changed_status_)
        {
            this->changed_status_ = false;
            this->reportStatus();
            clear_changes();
            pause_timestamp = millis();
            Pext.digitalWrite(lasers_pin, LOW);
        }
        if (millis() - pause_timestamp > pause_timeout) {
            check_data();
            refresh_puzzle();
        }
    }

    void LaserBarier::onDefault() {
        if (this->changed_status_)
        {
            this->changed_status_ = false;
            this->reportStatus();
            clear_changes();
        }
        check_data();
        if (puzzle_state == VIRUS) {
            show_virus();
        } else {
            init();
        }
    }

    void LaserBarier::onFinished() {
        if (this->changed_status_)
        {
            this->changed_status_ = false;
            this->reportStatus();
            clear_changes();
            animation_timestamp = millis();
        }
        check_data();
        if (puzzle_state == VIRUS)
            show_virus();
        else {
            show_success();
        }
    }

    void LaserBarier::onManualFinished() {
        if (this->changed_status_)
        {
            this->changed_status_ = false;
            this->reportStatus();
            clear_changes();
            animation_timestamp = millis();
        }

        check_data();
        if (puzzle_state == VIRUS) {
            show_virus();
        } else {
            show_success();
        }
    }

    void LaserBarier::init() {
        animation_timestamp = 0;
        pause_timestamp = 0;
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
                this->setStatus(FINISHED_STATUS);
                break;
            }
            case INCORRECT_ORDER: {
                show_failure();
                if (animation_timestamp == 0) {
                    puzzle_state = INITIAL;
                }
                break;
            }
            case VIRUS: {
                show_virus();
                break;
            }
        }
    }

    void LaserBarier::check_data() {
        if (!data.empty()) {
            StaticJsonDocument<SEFL::DOC_SIZE> doc;

            DeserializationError error = deserializeJson(doc, data.front());
            if (error) {
                Logger::notice(F("deserializeJson() failed: "));
                Logger::notice(error.f_str());
                return;
            }

            JsonVariant virus = doc["virus"];
            if (!virus.isNull()) {
                virus == 1 ? puzzle_state = VIRUS : puzzle_state = INITIAL;
            }

            data.remove(0);
        }
    }

    void LaserBarier::read_barrier() {

        for(int i = 0; i < barrier_size; i++) {
            bool temp = Mext.digitalRead(barrier[i].sensor_pin);
            if (!temp && barrier[i].order_number < curr_cross) {
                Logger::notice("Incorrect", "stopped crossing");
                puzzle_state = INCORRECT_ORDER;
                animation_timestamp = millis();
                curr_cross = 0;
                clearCurrentOrder();
                return;
            } else if (temp && barrier[i].crossed_order != curr_cross - 1
            && barrier[i].crossed_order < 0) {
                Logger::notice("barrier", barrier[i].order_number);
                Logger::notice("curr_cross", curr_cross);
                if (barrier[i].order_number != curr_cross)
                {
                    Logger::notice("Incorrect order");
                    puzzle_state = INCORRECT_ORDER;
                    animation_timestamp = millis();
                    curr_cross = 0;
                    clearCurrentOrder();
                    return;
                }
                barrier[i].crossed_order = curr_cross;
                Pext.digitalWrite(segments_pins[curr_cross], LOW);
                curr_cross++;
            }

        }

        if (curr_cross == barrier_size) {
            Logger::notice("Correct");
            puzzle_state = CORRECT_ORDER;
            animation_timestamp = millis();
            curr_cross = 0;
            clearCurrentOrder();
        }
    }

    void LaserBarier::show_virus() {

        Pext.digitalWrite(color_pins.red, HIGH);
        Pext.digitalWrite(color_pins.blue, LOW);
        Pext.digitalWrite(color_pins.green, LOW);

        for(int i = 0; i < segments_size; i++) {
            Pext.analogWrite(segments_pins[i], (sin(millis()*TWO_PI/500)+1)*2048);
        }
    }

    void LaserBarier::show_success() {
        if (millis()-animation_timestamp > finished_puzzle_timeout) {
            Pext.digitalWrite(color_pins.green, LOW);
            for(int i = 0; i < segments_size; i++) {
                Pext.digitalWrite(segments_pins[i], HIGH);
            }
            Pext.digitalWrite(code_sign_pin, HIGH);
            animation_timestamp = 0;
            return;
        }

        if (millis()-animation_timestamp > correct_animation_timeout &&
        millis()-animation_timestamp < finished_puzzle_timeout) {
            Pext.digitalWrite(color_pins.blue, LOW);
            Pext.digitalWrite(color_pins.green, HIGH);

            for(int i = 0; i < segments_size; i++) {
                Pext.digitalWrite(segments_pins[i], LOW);
            }
            Pext.digitalWrite(code_sign_pin, HIGH);
            return;
        }


        Pext.digitalWrite(lasers_pin, HIGH);

        auto state=(millis()-animation_timestamp)/(correct_animation_timeout/barrier_size/20)%barrier_size;
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

    void LaserBarier::clear_changes() {
        Pext.digitalWrite(lasers_pin, HIGH);

        Pext.digitalWrite(code_sign_pin, LOW);

        for(int i = 0; i < segments_size; i++) {
            Pext.digitalWrite(segments_pins[i], HIGH);
        }

        Pext.digitalWrite(color_pins.red, LOW);
        Pext.digitalWrite(color_pins.green, LOW);
        Pext.digitalWrite(color_pins.blue, LOW);


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
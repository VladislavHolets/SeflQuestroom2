//
// Created by vladi on 01.11.2022.
//

#include "generator.h"

namespace SEFL {
    Generator::Generator(MQTTClient &mqtt, const char *name, uint8_t resetStatus, const char *placement,
                         const char *inTopic, const char *outTopic, Language language) : Quest_Basic_Client(mqtt, name,
                                                                                                            resetStatus,
                                                                                                            placement,
                                                                                                            inTopic,
                                                                                                            outTopic,
                                                                                                            language) {
        puzzle_status=INITIAL;
        wires_pins= nullptr;
        correct_pattern= nullptr;
        current_pattern= nullptr;
        motors_pins_size=-1;
        wires_pins_size=-1;
        frame_light_pin=-1;
        generator_motors_pins= nullptr;
        generator_leds_pin=-1;
        motors_enable_pin=-1;
        animation_timestamp=0;
        correct_animation_timeout=1500;
        solved_animation_timeout=10000;
        second_motor_start_time = 2000;
        last_motor_start_time = 5000;
    }

    void Generator::setWiresPins(const uint8_t *wiresPins,uint8_t size) {
        if(wires_pins!= nullptr){
            delete [] wires_pins;
            delete [] correct_pattern;
            delete [] current_pattern;
        }
        wires_pins_size=size;
        wires_pins=new uint8_t[wires_pins_size];
        correct_pattern=new bool[wires_pins_size];
        current_pattern=new bool[wires_pins_size];
        for (int i = 0; i < wires_pins_size; ++i) {
            wires_pins[i]=wiresPins[i];
            pinMode(Mext.getCi(),INPUT_PULLUP);
            current_pattern[i]=Mext.digitalRead(wires_pins[i]);
        }
    }

    void Generator::setCorrectPattern(const uint8_t *correctPattern,uint8_t size) {
        if(size!=wires_pins_size){
            return;
        }
        for (int i = 0; i < wires_pins_size; ++i) {
            correct_pattern[i]=correctPattern[i];
        }
    }

    void Generator::onActive() {
        if(isChangedStatus()){
            unsetChangedStatus();
            reportStatus();
            puzzle_status=INITIAL;
        }
        if(puzzle_status==INITIAL) {
            scan_inputs();
        }
        refresh_outputs();
        if(puzzle_status==SOLVED){
            this->setStatus(FINISHED_STATUS);
        }
    }

    void Generator::onDefault() {
        if(isChangedStatus()){
            unsetChangedStatus();
            reportStatus();
            puzzle_status=INITIAL;
            refresh_outputs();

            //magnet 3 , light 2
            Pext.digitalWrite(2, HIGH);
            Pext.digitalWrite(3, LOW);

        }
    }

    void Generator::onFinished() {
        if(isChangedStatus()){
            unsetChangedStatus();
            reportStatus();
            refresh_outputs();

            //magnet 3 , light 2
            Pext.digitalWrite(2, LOW);
            Pext.digitalWrite(3, HIGH);
        }

    }

    void Generator::onManualFinished() {
        if(isChangedStatus()) {
            unsetChangedStatus();
            reportStatus();
            puzzle_status = SOLVING;
            animation_timestamp=millis();
        }
        if(puzzle_status==SOLVED) {
            Pext.digitalWrite(2, LOW);
            Pext.digitalWrite(3, HIGH);
        }
        refresh_outputs();
    }

    void Generator::scan_inputs() {
        bool solved_flag=true;
        pinMode(Mext.getCi(),INPUT_PULLUP);
        for (int i = 0; i < wires_pins_size; ++i) {
            bool temp = Mext.digitalRead(wires_pins[i]);
            if(temp==correct_pattern[i] && temp!=current_pattern[i]){
                puzzle_status=CORRECT_INSERTION;
                animation_timestamp=millis();
            }
            current_pattern[i]=temp;
            if(current_pattern[i]!=correct_pattern[i]){
                solved_flag=false;
            }
        }
        if(solved_flag==true){
            puzzle_status=SOLVING;
            animation_timestamp=millis();
        }
    }

    void Generator::refresh_outputs() {
        if(generator_leds_pin==-1 || generator_motors_pins == nullptr || frame_light_pin==-1){
            return;
        }
        switch (puzzle_status) {
            case INITIAL:{

                Pext.digitalWrite(generator_leds_pin,HIGH);

                Pext.digitalWrite(motors_enable_pin, HIGH);
                for(int i = 0; i < motors_pins_size; i++) {
                    Pext.digitalWrite(generator_motors_pins[i], LOW);
                }
                Pext.digitalWrite(frame_light_pin,HIGH);
                break;
            }
            case CORRECT_INSERTION:{
                if(millis()-animation_timestamp<correct_animation_timeout){
                    Pext.digitalWrite(frame_light_pin,(millis()-animation_timestamp)/300%2);
                }else{
                    puzzle_status=INITIAL;
                }
                break;
            }
            case SOLVED:{
                Pext.digitalWrite(generator_leds_pin,LOW);
                Pext.digitalWrite(frame_light_pin,LOW);
                break;
            }

            case SOLVING:{
                uint32_t current = millis()-animation_timestamp;
                if(current < solved_animation_timeout){
                    Pext.digitalWrite(generator_leds_pin,LOW);
                    Pext.digitalWrite(frame_light_pin,LOW);
                    Pext.digitalWrite(motors_enable_pin, LOW);

                    Pext.digitalWrite(generator_motors_pins[0], HIGH);
                    if(current > second_motor_start_time) {
                        Pext.digitalWrite(generator_motors_pins[1], HIGH);
                    }
                    if (current > last_motor_start_time) {
                        Pext.digitalWrite(generator_motors_pins[2], HIGH);
                    }

                }else{
                    puzzle_status=SOLVED;
                    for(int i = 0; i < motors_pins_size; i++) {
                        Pext.digitalWrite(generator_motors_pins[i], LOW);
                    }
                    Pext.digitalWrite(motors_enable_pin, HIGH);
                }

                break;
            }

        }
    }

    void Generator::setCorrectAnimationTimeout(uint32_t correctAnimationTimeout) {
        correct_animation_timeout = correctAnimationTimeout;
    }

    void Generator::setSolvedAnimationTimeout(uint32_t solvedAnimationTimeout) {
        solved_animation_timeout = solvedAnimationTimeout;
    }

    void Generator::setFrameLightPin(uint8_t frameLightPin) {
        frame_light_pin = frameLightPin;
    }

    void Generator::setGeneratorMotorsPin(const int16_t * generatorMotorsPins, int16_t size) {
        if(generatorMotorsPins!= nullptr){
            delete [] generator_motors_pins;
        }
        motors_pins_size=size;
        generator_motors_pins = new int16_t[motors_pins_size];

        for (int i = 0; i < motors_pins_size; i++) {
            generator_motors_pins[i]=generatorMotorsPins[i];
        }
    }

    void Generator::setGeneratorLedsPin(uint8_t generatorLedsPin) {
        generator_leds_pin = generatorLedsPin;
    }

    void Generator::setMotorsEnablePin(int16_t motorsEnablePin) {
        motors_enable_pin = motorsEnablePin;
    }
} // SEFL
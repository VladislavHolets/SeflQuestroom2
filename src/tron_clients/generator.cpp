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
        }
        scan_inputs();
        refresh_outputs();

    }

    void Generator::onDefault() {
        if(isChangedStatus()){
            unsetChangedStatus();
            reportStatus();
            puzzle_status=INITIAL;
            refresh_outputs();
        }
    }

    void Generator::onFinished() {
        if(isChangedStatus()){
            unsetChangedStatus();
            reportStatus();
            refresh_outputs();
        }
    }

    void Generator::onManualFinished() {
        onFinished();
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
            puzzle_status=SOLVED;
            animation_timestamp=millis();
        }
    }

    void Generator::refresh_outputs() {
        switch (puzzle_status) {
            case INITIAL:{

                Pext.digitalWrite(generator_leds_pin,LOW);
                Pext.digitalWrite(generator_motors_pin,LOW);
                Pext.digitalWrite(frame_light_pin,LOW);
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
                Pext.digitalWrite(generator_leds_pin,HIGH);
                Pext.digitalWrite(frame_light_pin,HIGH);
                break;
            }

            case SOLVING:{
                if(millis()-animation_timestamp<solved_animation_timeout){
                    Pext.digitalWrite(generator_leds_pin,HIGH);
                    Pext.digitalWrite(generator_motors_pin,HIGH);
                    Pext.digitalWrite(frame_light_pin,HIGH);
                }else{
                    puzzle_status=SOLVED;
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

    void Generator::setGeneratorMotorsPin(uint8_t generatorMotorsPin) {
        generator_motors_pin = generatorMotorsPin;
    }

    void Generator::setLedsPin(uint8_t ledsPin) {
        generator_leds_pin = ledsPin;
    }
} // SEFL
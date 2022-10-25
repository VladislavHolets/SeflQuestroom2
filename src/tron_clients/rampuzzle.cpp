//
// Created by vladi on 22.10.2022.
//

#include "rampuzzle.h"

namespace SEFL {
    RAMPuzzle::RAMPuzzle(MQTTClient &mqtt, const char *name,
                         uint8_t reset_status, const char *placement, const char *in_topic,
                         const char *out_topic, SEFL::Language language) : Quest_Basic_Client(mqtt, name, reset_status, placement, in_topic,
                                                                                              out_topic, language) {
    sensor_pins= nullptr;
    led_pins= nullptr;
    state=UNTRIGGERED;
    sensor_pins_size=0;
    led_pins_size=0;
    }

    void RAMPuzzle::onActive() {
        if (this->changed_status_)
        {
            this->changed_status_ = false;
            this->reportStatus();
            state=UNTRIGGERED;
        }
        if(state==UNTRIGGERED){
            if(millis()/1000%2){
                resetLeds();
            }else {
                setLeds();
            }
            pinMode(Mext.getCi(),INPUT_PULLUP);
            for (int i = 0; i < sensor_pins_size; ++i) {
                if(!Mext.digitalRead(sensor_pins[i])){
                    state=TRIGGERED;
                }
            }
        }else if(state==TRIGGERED){
            updateLeds();
            if (checkResults()) {
                this->setStatus(FINISHED_STATUS);
            }
        }
    }

    void RAMPuzzle::onDefault() {
        if (this->changed_status_)
        {
            this->changed_status_ = false;
            this->reportStatus();
            resetLeds();
            state=UNTRIGGERED;
        }
    }

    void RAMPuzzle::onFinished() {
        if (this->changed_status_)
        {
            this->changed_status_ = false;
            this->reportStatus();
            setLeds();

        }
    }

    void RAMPuzzle::onManualFinished() {
        onFinished();
    }

    void RAMPuzzle::setSensorPins(const uint8_t *sensorPins,uint8_t sensorPinsSize) {
        if(sensor_pins!= nullptr){
            delete sensor_pins;
        }
        sensor_pins_size=sensorPinsSize;
        sensor_pins=new uint8_t [sensor_pins_size];
        for (int i = 0; i < sensor_pins_size; ++i) {
            sensor_pins[i]=sensorPins[i];
        }
    }

    void RAMPuzzle::setLedPins(const uint8_t *ledPins,uint8_t ledPinsSize) {
        if(led_pins!= nullptr){
            delete led_pins;
        }
        led_pins_size=ledPinsSize;
        led_pins=new uint8_t [led_pins_size];
        for (int i = 0; i < led_pins_size; ++i) {
            led_pins[i]=ledPins[i];
        }
    }

    void RAMPuzzle::updateLeds() {
        if(sensor_pins_size!=led_pins_size){
            return;
        }
        pinMode(Mext.getCi(),INPUT_PULLUP);
        for (int i = 0; i < sensor_pins_size; ++i) {
            Pext.digitalWrite(led_pins[i],!Mext.digitalRead(sensor_pins[i])==LOW);
        }
    }

    bool RAMPuzzle::checkResults() {
        if(sensor_pins_size!=led_pins_size){
            return false;
        }
        pinMode(Mext.getCi(),INPUT_PULLUP);
        for (int i = 0; i < sensor_pins_size; ++i) {
            if(Mext.digitalRead(sensor_pins[i])){
                return false;
            }
        }
        return true;
    }

    void RAMPuzzle::resetLeds() {
        if(sensor_pins_size!=led_pins_size){
            return;
        }
        for (int i = 0; i < sensor_pins_size; ++i) {
            Pext.digitalWrite(led_pins[i],HIGH);
        }
    }

    void RAMPuzzle::setLeds() {
        if(sensor_pins_size!=led_pins_size){
            return;
        }
        for (int i = 0; i < sensor_pins_size; ++i) {
            Pext.digitalWrite(led_pins[i],LOW);
        }
    }
} // SEFL
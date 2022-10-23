//
// Created by vladi on 22.10.2022.
//

#include "rampuzzle.h"

namespace SEFL {
    RAMPuzzle::RAMPuzzle(MQTTClient &mqtt, const char *name,
                         uint8_t reset_status, const char *placement, const char *in_topic,
                         const char *out_topic, SEFL::Language language) : Quest_Basic_Client(mqtt, name, reset_status, placement, in_topic,
                                                                                              out_topic, language) {

    }

    void RAMPuzzle::onActive() {
        if (this->changed_status_)
        {
            this->changed_status_ = false;
            this->reportStatus();
        }
    }

    void RAMPuzzle::onDefault() {
        if (this->changed_status_)
        {
            this->changed_status_ = false;
            this->reportStatus();
        }
    }

    void RAMPuzzle::onFinished() {
        if (this->changed_status_)
        {
            this->changed_status_ = false;
            this->reportStatus();
        }
    }

    void RAMPuzzle::onManualFinished() {
        onFinished();
    }

    void RAMPuzzle::setSensorPins(uint8_t *sensorPins,uint8_t sensorPinsSize) {
        if(sensor_pins!= nullptr){
            delete sensor_pins;
        }

    }

    void RAMPuzzle::setLedPins(uint8_t *ledPins,uint8_t ledPinsSize) {
        if(led_pins!= nullptr){
            delete led_pins;
        }
    }
} // SEFL
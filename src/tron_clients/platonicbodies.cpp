//
// Created by vladi on 21.03.2023.
//

#include "platonicbodies.h"

namespace SEFL {
    PlatonicBodies::PlatonicBodies(MQTTClient &mqtt, const char *name, uint8_t resetStatus, const char *placement,
                                   const char *inTopic, const char *outTopic, Language language) : Quest_Basic_Client(
            mqtt, name, resetStatus, placement, inTopic, outTopic, language) {
        sensor_signal_inverted=true;
        sensor_pin=-1;
        laser_pin=-1;
    }

    void PlatonicBodies::onActive() {
        if(isChangedStatus()){
            unsetChangedStatus();
            Quest_Basic_Client::reportStatus();
            Pext.digitalWrite(laser_pin,HIGH);
        }
        if(Mext.digitalRead(sensor_pin)==sensor_signal_inverted){
            this->setStatus(FINISHED_STATUS);
        }
    }

    void PlatonicBodies::onDefault() {
        if(isChangedStatus()){
            unsetChangedStatus();
            Quest_Basic_Client::reportStatus();
            Pext.digitalWrite(laser_pin,LOW);
        }
    }

    void PlatonicBodies::onFinished() {
        if(isChangedStatus()){
            unsetChangedStatus();
            Quest_Basic_Client::reportStatus();
            Pext.digitalWrite(laser_pin,LOW);
        }
    }

    void PlatonicBodies::onManualFinished() {
        if(isChangedStatus()){
            unsetChangedStatus();
            Quest_Basic_Client::reportStatus();

            Pext.digitalWrite(laser_pin,LOW);
        }
    }

    void PlatonicBodies::setSensorPin(int16_t sensorPin) {
        sensor_pin = sensorPin;
    }

    void PlatonicBodies::setLaserPin(int16_t laserPin) {
        laser_pin = laserPin;
    }

    void PlatonicBodies::setSensorSignalInverted(bool sensorSignalInverted) {
        sensor_signal_inverted = sensorSignalInverted;
    }
} // SEFL
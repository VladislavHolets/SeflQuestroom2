//
// Created by vladi on 24.03.2023.
//

#include "troncubechest.h"

namespace SEFL {
    TronCubeChest::TronCubeChest(MQTTClient &mqtt, const char *name, uint8_t resetStatus, const char *placement,
                                 const char *inTopic, const char *outTopic, Language language) : Quest_Actuator_Client(
            mqtt, name, resetStatus, placement, inTopic, outTopic, language) {
        led_pin=-1;
        magnet_pin=-1;
        inverted_=false;

    }

    void TronCubeChest::onOn() {
        if(isChangedStatus()){
            Pext.digitalWrite(led_pin,LOW!=inverted_);
            Pext.digitalWrite(magnet_pin,HIGH!=inverted_);

            unsetChangedStatus();
            Quest_Actuator_Client::reportStatus();
        }
    }

    void TronCubeChest::onOff() {
        if(isChangedStatus()){
            Pext.digitalWrite(led_pin,HIGH!=inverted_);
            Pext.digitalWrite(magnet_pin,LOW!=inverted_);
            unsetChangedStatus();
            Quest_Actuator_Client::reportStatus();
        }

    }

    void TronCubeChest::setMagnetPin(int16_t magnetPin) {
        magnet_pin = magnetPin;
    }

    void TronCubeChest::setLedPin(int16_t ledPin) {
        led_pin = ledPin;
    }

    void TronCubeChest::setInverted(bool inverted) {
        inverted_ = inverted;
    }
} // SEFL
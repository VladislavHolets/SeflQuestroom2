//
// Created by vladi on 25.11.2022.
//

#include "andpuzzle.h"

namespace SEFL {

    ANDPuzzle::ANDPuzzle(MQTTClient &mqtt, const char *name, uint8_t resetStatus, const char *placement,
                         const char *inTopic, const char *outTopic, Language language) : Quest_Basic_Client(mqtt, name,
                                                                                                             resetStatus,
                                                                                                             placement,
                                                                                                             inTopic,
                                                                                                             outTopic,
                                                                                                             language) {
        adapter_ = nullptr;
        led_on_timeout=60000;
        led_on_timestamp=0;
    }


    void ANDPuzzle::setAdapter(TronLegacyAdapter &adapter) {
        if (adapter_ != nullptr) {
            delete adapter_;
        }
        adapter_ = new TronLegacyAdapter;
        adapter_->setManualPin(adapter.getManualPin());
        adapter_->setResetPin(adapter.getResetPin());
        adapter_->setSolvedStatePin(adapter.getSolvedStatePin());
        adapter_->setPowerPin(adapter.getPowerPin());

    }

    void ANDPuzzle::onActive() {
        if(isChangedStatus()){
            adapter_->setDevice();
            Pext.digitalWrite(adapter_->getPowerPin(),LOW);
            adapter_->unSolveDevice();
            unsetChangedStatus();
            Quest_Basic_Client::reportStatus();
        }
        pinMode(Mext.getCi(),INPUT);
        //Logger::notice("ANDPuzzle",Mext.analogRead(adapter_->getSolvedStatePin()));
        if(Mext.analogRead(adapter_->getSolvedStatePin())>380){
            this->setStatus(FINISHED_STATUS);
        }
    }

    void ANDPuzzle::onDefault() {
        if(isChangedStatus()){
            adapter_->resetDevice();
            Pext.digitalWrite(adapter_->getPowerPin(),HIGH);
            adapter_->unSolveDevice();
            unsetChangedStatus();
            Quest_Basic_Client::reportStatus();
        }
    }

    void ANDPuzzle::onFinished() {
        if(isChangedStatus()){
            unsetChangedStatus();
            Quest_Basic_Client::reportStatus();
            led_on_timestamp=millis();
        }
        if(led_on_timestamp && (millis()-led_on_timestamp)>led_on_timeout){
            adapter_->resetDevice();
            Pext.digitalWrite(adapter_->getPowerPin(),HIGH);
            adapter_->unSolveDevice();
            led_on_timestamp=0;
        }
    }

    void ANDPuzzle::onManualFinished() {
        if(isChangedStatus()){
            unsetChangedStatus();
            Quest_Basic_Client::reportStatus();
            adapter_->resetDevice();
            Pext.digitalWrite(adapter_->getPowerPin(),HIGH);
            adapter_->unSolveDevice();
        }
    }

    uint32_t ANDPuzzle::getLedOnTimeout() const {
        return led_on_timeout;
    }

    void ANDPuzzle::setLedOnTimeout(uint32_t ledOnTimeout) {
        led_on_timeout = ledOnTimeout;
    }

} // SEFL
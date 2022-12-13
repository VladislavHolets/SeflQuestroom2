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
    }


    void ANDPuzzle::setAdapter(TronLegacyAdapter &adapter) {
        if (adapter_ != nullptr) {
            delete adapter_;
        }
        adapter_ = new TronLegacyAdapter;
        adapter_->setManualPin(adapter.getManualPin());
        adapter_->setResetPin(adapter.getResetPin());
        adapter_->setSolvedStatePin(adapter.getSolvedStatePin());

    }

    void ANDPuzzle::onActive() {
        if(isChangedStatus()){
            adapter_->setDevice();
            unsetChangedStatus();
            Quest_Basic_Client::reportStatus();
        }
        Logger::notice("ANDPuzzle",Mext.analogRead(adapter_->getSolvedStatePin()));
    }

    void ANDPuzzle::onDefault() {
        if(isChangedStatus()){
            adapter_->resetDevice();
            unsetChangedStatus();
            Quest_Basic_Client::reportStatus();
        }
    }

    void ANDPuzzle::onFinished() {
        if(isChangedStatus()){
            adapter_->resetDevice();
            unsetChangedStatus();
            Quest_Basic_Client::reportStatus();
        }
    }

    void ANDPuzzle::onManualFinished() {
        onFinished();
    }

} // SEFL
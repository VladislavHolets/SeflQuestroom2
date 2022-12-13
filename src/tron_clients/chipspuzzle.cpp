//
// Created by vladi on 04.12.2022.
//

#include "chipspuzzle.h"

namespace SEFL {
    void ChipsPuzzle::onActive() {

        static uint32_t reset_timestamp = 0;
        if (isChangedStatus()) {
            reset_timestamp = millis();
            adapter_->resetDevice();
            adapter_->unSolveDevice();
            adapter_->setDevice();
            unsetChangedStatus();
            Quest_Basic_Client::reportStatus();
            return;
        }
        if(reset_timestamp && millis()-reset_timestamp>adapter_->getResetTimeout()) {
            Quest_Basic_Client::reportStatus();
            reset_timestamp=0;
        }else if(reset_timestamp){
            return;
        }
        if(adapter_->isSolved()){
            this->setStatus(FINISHED_STATUS);
        }
    }

    void ChipsPuzzle::onDefault() {
        static uint32_t reset_timestamp = 0;
        if (isChangedStatus()) {
            reset_timestamp = millis();
            adapter_->resetDevice();
            adapter_->unSolveDevice();
            adapter_->setDevice();
            unsetChangedStatus();
        }
        if(reset_timestamp && millis()-reset_timestamp>adapter_->getResetTimeout()) {
            Quest_Basic_Client::reportStatus();
            this->adapter_->resetDevice();
            reset_timestamp=0;
        }
    }

    void ChipsPuzzle::onFinished() {
        if (isChangedStatus()) {
            adapter_->setDevice();
            adapter_->unSolveDevice();
            adapter_->resetDevice();
            unsetChangedStatus();
            Quest_Basic_Client::reportStatus();
        }
        Logger::notice("ChipsPuzzle", Mext.digitalRead(adapter_->getSolvedStatePin()));

    }

    void ChipsPuzzle::onManualFinished() {
        if (isChangedStatus()) {
            adapter_->solveDevice();
            unsetChangedStatus();
            Quest_Basic_Client::reportStatus();
        }
        Logger::notice("ChipsPuzzle", Mext.digitalRead(adapter_->getSolvedStatePin()));

    }

    void ChipsPuzzle::setAdapter(TronLegacyAdapter &adapter) {
        if (adapter_ != nullptr) {
            delete adapter_;
        }
        adapter_ = new TronLegacyAdapter;
        adapter_->setManualPin(adapter.getManualPin());
        adapter_->setResetPin(adapter.getResetPin());
        adapter_->setSolvedStatePin(adapter.getSolvedStatePin());
    }

    ChipsPuzzle::ChipsPuzzle(MQTTClient &mqtt, const char *name, uint8_t resetStatus, const char *placement,
                             const char *inTopic, const char *outTopic, Language language)
            : Quest_Basic_Client(mqtt, name, resetStatus, placement, inTopic, outTopic, language) {
        adapter_ = nullptr;
    }
} // SEFL
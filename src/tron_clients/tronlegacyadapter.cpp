//
// Created by rockr on 25.10.2022.
//

#include "tronlegacyadapter.h"

uint8_t TronLegacyAdapter::getResetPin() const {
    return reset_pin;
}

uint8_t TronLegacyAdapter::getManualPin() const {
    return manual_pin;
}

uint8_t TronLegacyAdapter::getSolvedStatePin() const {
    return solved_state_pin;
}

void TronLegacyAdapter::setResetPin(uint8_t resetPin) {
    reset_pin = resetPin;
}

void TronLegacyAdapter::setManualPin(uint8_t manualPin) {
    manual_pin = manualPin;
}

void TronLegacyAdapter::setSolvedStatePin(uint8_t solvedStatePin) {
    solved_state_pin = solvedStatePin;
}

void TronLegacyAdapter::resetDevice() {
    if(reset_pin != -1)
        Pext.digitalWrite(reset_pin, LOW);
}

void TronLegacyAdapter::setDevice() {
    if(reset_pin != -1)
        Pext.digitalWrite(reset_pin, HIGH);
}

void TronLegacyAdapter::solveDevice() {
    if(manual_pin != -1)
        Pext.digitalWrite(manual_pin, HIGH);
}

void TronLegacyAdapter::unSolveDevice() {
    if(manual_pin != -1)
        Pext.digitalWrite(manual_pin, LOW);
}

bool TronLegacyAdapter::isSolved() {
    if (solved_state_pin != -1) {
        pinMode(Mext.getCi(), INPUT);
        return !Mext.digitalRead(solved_state_pin);
    }
    return false;
}

TronLegacyAdapter::TronLegacyAdapter() {
    this->reset_pin = -1;
    this->manual_pin = -1;
    this->solved_state_pin = -1;
    this->reset_timeout=2000;
}

TronLegacyAdapter::~TronLegacyAdapter() {

}

uint32_t TronLegacyAdapter::getResetTimeout() const {
    return reset_timeout;
}

void TronLegacyAdapter::setResetTimeout(uint32_t resetTimeout) {
    reset_timeout = resetTimeout;
}

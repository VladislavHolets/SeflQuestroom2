//
// Created by rockr on 25.10.2022.
//

#include "tronlegacyadapter.h"

uint8_t tronlegacyadapter::getResetPin() const {
    return reset_pin;
}

uint8_t tronlegacyadapter::getManualPin() const {
    return manual_pin;
}

uint8_t tronlegacyadapter::getSolvedStatePin() const {
    return solved_state_pin;
}

void tronlegacyadapter::setResetPin(uint8_t resetPin) {
    reset_pin = resetPin;
}

void tronlegacyadapter::setManualPin(uint8_t manualPin) {
    manual_pin = manualPin;
}

void tronlegacyadapter::setSolvedStatePin(uint8_t solvedStatePin) {
    solved_state_pin = solvedStatePin;
}

void tronlegacyadapter::resetDevice() {
    if(reset_pin != -1)
        Pext.digitalWrite(reset_pin, LOW);
}

void tronlegacyadapter::setDevice() {
    if(reset_pin != -1)
        Pext.digitalWrite(reset_pin, HIGH);
}

void tronlegacyadapter::solveDevice() {
    if(manual_pin != -1)
        Pext.digitalWrite(manual_pin, LOW);
}

void tronlegacyadapter::unSolveDevice() {
    if(manual_pin != -1)
        Pext.digitalWrite(manual_pin, HIGH);
}

bool tronlegacyadapter::is_solved() {
    if (solved_state_pin != -1) {
        pinMode(Mext.getCi(), INPUT_PULLUP);
        return !Mext.digitalRead(solved_state_pin);
    }
}

tronlegacyadapter::tronlegacyadapter() {
    this->reset_pin = -1;
    this->manual_pin = -1;
    this->solved_state_pin = -1;
}

TronLegacyAdapter::~TronLegacyAdapter() {

}

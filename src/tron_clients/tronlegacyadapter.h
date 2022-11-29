//
// Created by rockr on 25.10.2022.
//

#ifndef SEFLQUESTROOM2_TRONLEGACYADAPTER_H
#define SEFLQUESTROOM2_TRONLEGACYADAPTER_H

#include "sefl_quest_core.h"

#include <cstdint>


class TronLegacyAdapter {

private:
    int8_t reset_pin;
    int8_t manual_pin;
    int8_t solved_state_pin;

public:
    TronLegacyAdapter();
    virtual ~TronLegacyAdapter();

    void setResetPin(uint8_t resetPin);
    void setManualPin(uint8_t manualPin);
    void setSolvedStatePin(uint8_t solvedStatePin);

    uint8_t getResetPin() const;
    uint8_t getManualPin() const;
    uint8_t getSolvedStatePin() const;

    void resetDevice();
    void setDevice();
    void solveDevice();
    void unSolveDevice();
    bool isSolved();

};


#endif //SEFLQUESTROOM2_TRONLEGACYADAPTER_H

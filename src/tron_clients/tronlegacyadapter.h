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
    int16_t power_pin;
    uint32_t reset_timeout;
public:
    TronLegacyAdapter();
    virtual ~TronLegacyAdapter();

    uint32_t getResetTimeout() const;

    void setResetTimeout(uint32_t resetTimeout);

    void setResetPin(uint8_t resetPin);
    void setManualPin(uint8_t manualPin);
    void setSolvedStatePin(uint8_t solvedStatePin);

    uint8_t getResetPin() const;
    uint8_t getManualPin() const;
    uint8_t getSolvedStatePin() const;

    int16_t getPowerPin() const;

    void setPowerPin(int16_t powerPin);

    void resetDevice();
    void setDevice();
    void solveDevice();
    void unSolveDevice();
    bool isSolved();

};


#endif //SEFLQUESTROOM2_TRONLEGACYADAPTER_H

//
// Created by vladi on 19.11.2022.
//

#include "pextservo.h"

namespace SEFL {
    PextServo::PextServo() {
        pin_=-1;
        current_value_=90;
        Pext.getHandler()->setPWMFreqServo();
    }

    uint8_t PextServo::attach(uint8_t pin) {
        if(pin>=0 && pin<16){
        this->pin_=pin;
        return 0;
        }
        return 1;
    }

    void PextServo::detach() {
        if(pin_>=0 && pin_<16) {
            Pext.getHandler()->setChannelPWM(pin_, 0);
        }
        pin_=-1;
    }

    void PextServo::write(int value) {
        if(value>=0 && value<=180 && pin_>=0){
            this->current_value_=value;
            Pext.getHandler()->setChannelPWM(pin_,calculator.pwmForAngle(value-90));
        }
    }

    int PextServo::read() {
        return this->current_value_;
    }

    bool PextServo::attached() {
        return pin_>-1;
    }
} // SEFL
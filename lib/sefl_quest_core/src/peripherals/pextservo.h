//
// Created by vladi on 19.11.2022.
//

#ifndef SEFLQUESTROOM2_PEXTSERVO_H
#define SEFLQUESTROOM2_PEXTSERVO_H

#include <cinttypes>
#include "pwmpinsextender.h"
namespace SEFL {

    class PextServo {
        PCA9685_ServoEval calculator;
    public:
        PextServo();
        uint8_t attach(uint8_t pin);           // attach the given pin to the next free channel, sets pinMode, set angle value, returns channel number or 0 if failure
       void detach();
        void write(int value);             // value is to be in range [0;180] degrees
        int read();                        // returns current pulse width as an angle between 0 and 180 degrees
        bool attached();                   // return true if this servo is attached, otherwise false
    private:
        int16_t pin_;
        uint16_t current_value_{};
    };

} // SEFL

#endif //SEFLQUESTROOM2_PEXTSERVO_H

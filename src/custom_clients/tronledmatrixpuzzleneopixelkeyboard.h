//
// Created by vladi on 18.10.2022.
//

#ifndef SEFLQUESTROOM2_TRONLEDMATRIXPUZZLENEOPIXELKEYBOARD_H
#define SEFLQUESTROOM2_TRONLEDMATRIXPUZZLENEOPIXELKEYBOARD_H


#include <Adafruit_NeoPixel.h>
#include <sefl_quest_core.h>

namespace SEFL {

    class TronLEDMatrixPuzzleNeopixelKeyboard:public Quest_Basic_Client {
    public:
        TronLEDMatrixPuzzleNeopixelKeyboard(MQTTClient &mqtt, const char *name, uint8_t resetStatus,
                                            const char *placement, const char *inTopic=DEFAUT_IN_TOPIC, const char *outTopic=DEFAUT_OUT_TOPIC,
                                            Language language=ENG);

    };

} // SEFL

#endif //SEFLQUESTROOM2_TRONLEDMATRIXPUZZLENEOPIXELKEYBOARD_H

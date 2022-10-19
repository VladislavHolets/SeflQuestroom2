//
// Created by vladi on 18.10.2022.
//

#include "tronledmatrixpuzzleneopixelkeyboard.h"

namespace SEFL {
    TronLEDMatrixPuzzleNeopixelKeyboard::TronLEDMatrixPuzzleNeopixelKeyboard(MQTTClient &mqtt, const char *name,
                                                                             uint8_t resetStatus, const char *placement,
                                                                             const char *inTopic, const char *outTopic,
                                                                             Language language) : Quest_Basic_Client(
            mqtt, name, resetStatus, placement, inTopic, outTopic, language) {}
} // SEFL
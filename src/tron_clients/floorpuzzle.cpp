//
// Created by vladi on 13.02.2023.
//

#include "floorpuzzle.h"

namespace SEFL {
    FloorPuzzle::FloorPuzzle(MQTTClient &mqtt, const char *name, uint8_t resetStatus, const char *placement,
                             const char *inTopic, const char *outTopic, Language language) : Quest_Basic_Client(mqtt,
                                                                                                                name,
                                                                                                                resetStatus,
                                                                                                                placement,
                                                                                                                inTopic,
                                                                                                                outTopic,
                                                                                                                language) {}

    void FloorPuzzle::onActive() {

    }

    void FloorPuzzle::onDefault() {

    }

    void FloorPuzzle::onFinished() {

    }

    void FloorPuzzle::onManualFinished() {

    }
} // SEFL
//
// Created by vladi on 13.02.2023.
//

#ifndef SEFLQUESTROOM2_FLOORPUZZLE_H
#define SEFLQUESTROOM2_FLOORPUZZLE_H
#include "sefl_quest_core.h"
namespace SEFL {

    class FloorPuzzle: public Quest_Basic_Client {
        struct Panel{
            uint8_t sensor_pin;
            uint8_t led_pin;
        };
        enum
        void init_panels();
        void init_puzzle();

    public:
        FloorPuzzle(MQTTClient &mqtt, const char *name, uint8_t resetStatus, const char *placement, const char *inTopic=DEFAUT_IN_TOPIC, const char *outTopic=DEFAUT_OUT_TOPIC,
                    Language language=ENG);

        void onActive() override;

        void onDefault() override;

        void onFinished() override;

        void onManualFinished() override;
    };

} // SEFL

#endif //SEFLQUESTROOM2_FLOORPUZZLE_H

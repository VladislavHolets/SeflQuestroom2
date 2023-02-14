//
// Created by vladi on 16.12.2022.
//

#ifndef SEFLQUESTROOM2_STATUSBARHC595_H
#define SEFLQUESTROOM2_STATUSBARHC595_H
#include "sefl_quest_core.h"

namespace SEFL {

    class StatusBarHC595: public Quest_Actuator_Client {
        uint8_t *status_lamps_pins;
        int8_t status_lamps_size;
        bool *status_lamps_states;
        uint8_t status_lamps_chip;
        HC595_Driver *driver_;
        void refreshLeds();

    public:
        void setStatusLampsPins(const uint8_t *statusLampsPins, int8_t statusLampsSize);

        void setDriver(HC595_Driver &driver);
        void setStatusLampsChip(uint8_t statusLampsChip);

        StatusBarHC595(MQTTClient
        &mqtt,
        const char *name, uint8_t
        resetStatus,
        const char *placement,
        const char *inTopic = DEFAUT_IN_TOPIC,
        const char *outTopic = DEFAUT_OUT_TOPIC,
                Language
        language = ENG
        );

    public:
        void onOn() override;

        void onOff() override;
    };

} // SEFL

#endif //SEFLQUESTROOM2_STATUSBARHC595_H

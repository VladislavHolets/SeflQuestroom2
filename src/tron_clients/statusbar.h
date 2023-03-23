//
// Created by vladi on 28.10.2022.
//

#ifndef SEFLQUESTROOM2_STATUSBAR_H
#define SEFLQUESTROOM2_STATUSBAR_H
#include "sefl_quest_core.h"
namespace SEFL {

    class StatusBar: public Quest_Actuator_Client{
        uint8_t *status_lamps_pins;
        int8_t status_lamps_size;
        bool *status_lamps_states;
        void refreshLeds();
    public:
        void setStatusLampsPins(const uint8_t *statusLampsPins,int8_t statusLampsSize);

        StatusBar(MQTTClient &mqtt, const char *name, uint8_t resetStatus,
                  const char *placement, const char *inTopic=DEFAUT_IN_TOPIC, const char *outTopic=DEFAUT_OUT_TOPIC,
                  Language language=ENG);

    public:
        void onOn() override;

        void onOff() override;

    };

} // SEFL

#endif //SEFLQUESTROOM2_STATUSBAR_H

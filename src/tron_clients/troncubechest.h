//
// Created by vladi on 24.03.2023.
//

#ifndef SEFLQUESTROOM2_TRONCUBECHEST_H
#define SEFLQUESTROOM2_TRONCUBECHEST_H
#include "sefl_quest_core.h"
namespace SEFL {

    class TronCubeChest: public Quest_Actuator_Client {
        int16_t magnet_pin;
        int16_t led_pin;
        bool inverted_;
    public:
        TronCubeChest(MQTTClient &mqtt, const char *name, uint8_t resetStatus=1, const char *placement=DEFAULT_PLACEMENT,
                      const char *inTopic=DEFAUT_IN_TOPIC, const char *outTopic=DEFAUT_OUT_TOPIC, Language language=ENG);

        void onOn() override;

        void onOff() override;

        void setMagnetPin(int16_t magnetPin);

        void setLedPin(int16_t ledPin);

        void setInverted(bool inverted);
    };

} // SEFL

#endif //SEFLQUESTROOM2_TRONCUBECHEST_H

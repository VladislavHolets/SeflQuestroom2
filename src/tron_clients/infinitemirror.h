//
// Created by vladi on 14.03.2023.
//

#ifndef SEFLQUESTROOM2_INFINITEMIRROR_H
#define SEFLQUESTROOM2_INFINITEMIRROR_H
#include "sefl_quest_core.h"
namespace SEFL {

    class InfiniteMirror: public Quest_Actuator_Client{
        int16_t *led_cathodes;
        int8_t led_cathodes_size;
        uint32_t animation_start_timestamp;
        char led_mode;
        void cycleMode(char ledMode);
    public:
        InfiniteMirror(MQTTClient &mqtt, const char *name, uint8_t resetStatus=1, const char *placement=DEFAULT_PLACEMENT,
                       const char *inTopic=DEFAUT_IN_TOPIC, const char *outTopic=DEFAUT_OUT_TOPIC, Language language=ENG);

        int16_t *getLedCathodes() const;

        void setLedCathodes(int16_t *ledCathodes,int8_t size);

        void onOn() override;

        void onOff() override;
    };

} // SEFL

#endif //SEFLQUESTROOM2_INFINITEMIRROR_H

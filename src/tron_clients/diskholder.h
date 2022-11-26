//
// Created by vladi on 20.11.2022.
//

#ifndef SEFLQUESTROOM2_DISKHOLDER_H
#define SEFLQUESTROOM2_DISKHOLDER_H
#include "sefl_quest_core.h"
namespace SEFL {
    enum DiskHolderArrayType{
        DISPENCER,RECEIVER
    };
    struct HolderPins{
        int16_t servo_pin;
        int16_t led_pin;
        int16_t sensor_pin;
    };
    class DiskHolderArray:public Quest_Basic_Client {
        DiskHolderArrayType holdertype_;
        HolderPins *holders_;
        int16_t holders_size_;
        PextServo *holders_servos_;
    public:
        DiskHolderArray(MQTTClient &mqtt, const char *name, uint8_t resetStatus=1, const char *placement=DEFAULT_PLACEMENT,
                        const char *inTopic=DEFAUT_IN_TOPIC, const char *outTopic=DEFAUT_OUT_TOPIC, Language language=DEFAULT_LANGUAGE);
        bool check_disk(uint8_t index);
        void extract_disk(uint8_t index);
        void extract_disk();

        void setHolders(HolderPins *holders,uint8_t size);


    };

} // SEFL

#endif //SEFLQUESTROOM2_DISKHOLDER_H

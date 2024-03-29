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
        uint32_t attached_timestamp;
    };
    class DiskHolderArray:public Quest_Basic_Client {
        DiskHolderArrayType holdertype_;
        HolderPins *holders_;
        int16_t holders_size_;
        PextServo *holders_servos_;
        bool led_state_;
        uint8_t dispence_angle,receive_angle;

    public:
        DiskHolderArray(MQTTClient &mqtt, const char *name, uint8_t resetStatus=1, const char *placement=DEFAULT_PLACEMENT,
                        const char *inTopic=DEFAUT_IN_TOPIC, const char *outTopic=DEFAUT_OUT_TOPIC, Language language=DEFAULT_LANGUAGE);
        bool check_disk(uint8_t index);
        void dispence_disk(uint8_t index);
        void dispence_disk();
        void receive_disk(uint8_t index);
        void onActive() override;

        void onDefault() override;

        void onFinished() override;

        void onManualFinished() override;

        void set_led_state(bool led_state);
        void detach_servos(uint32_t timeout);
        void setHolders(HolderPins *holders,uint8_t size);

        DiskHolderArrayType getHoldertype() const;

        void setHoldertype(DiskHolderArrayType holdertype);

    };

} // SEFL

#endif //SEFLQUESTROOM2_DISKHOLDER_H

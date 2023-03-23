//
// Created by vladi on 21.03.2023.
//

#ifndef SEFLQUESTROOM2_PLATONICBODIES_H
#define SEFLQUESTROOM2_PLATONICBODIES_H
#include "sefl_quest_core.h"
namespace SEFL {

    class PlatonicBodies: public Quest_Basic_Client{
        int16_t sensor_pin;
        int16_t laser_pin;
        bool sensor_signal_inverted;
    public:
        PlatonicBodies(MQTTClient &mqtt, const char *name, uint8_t resetStatus=1, const char *placement=DEFAULT_PLACEMENT, const char *inTopic=DEFAUT_IN_TOPIC,
                       const char *outTopic=DEFAUT_OUT_TOPIC, Language language=ENG);

        void onActive() override;

        void onDefault() override;

        void onFinished() override;

        void onManualFinished() override;

        void setSensorPin(int16_t sensorPin);

        void setLaserPin(int16_t laserPin);

        void setSensorSignalInverted(bool sensorSignalInverted);
    };

} // SEFL

#endif //SEFLQUESTROOM2_PLATONICBODIES_H

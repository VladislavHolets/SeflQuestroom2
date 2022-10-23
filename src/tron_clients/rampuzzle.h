//
// Created by vladi on 22.10.2022.
//

#ifndef SEFLQUESTROOM2_RAMPUZZLE_H
#define SEFLQUESTROOM2_RAMPUZZLE_H
#include "sefl_quest_core.h"
namespace SEFL {

    class RAMPuzzle:Quest_Basic_Client {
        uint8_t * sensor_pins;
        uint8_t sensor_pins_size;
    public:
        void setSensorPins(uint8_t *sensorPins,uint8_t sensorPinsSize);

        void setLedPins(uint8_t *ledPins,uint8_t ledPinsSize);

    private:
        uint8_t *led_pins;
        uint8_t led_pins_size;
    public:
        RAMPuzzle(MQTTClient &mqtt, const char *name = DEFAULT_DEVICE_NAME,
                  uint8_t reset_status = 1, const char *placement = DEFAULT_PLACEMENT,
                  const char *in_topic = DEFAUT_IN_TOPIC, const char *out_topic = DEFAUT_OUT_TOPIC, SEFL::Language language = UKR);

    private:
        void onActive() override;

        void onDefault() override;

        void onFinished() override;

        void onManualFinished() override;
    };

} // SEFL

#endif //SEFLQUESTROOM2_RAMPUZZLE_H

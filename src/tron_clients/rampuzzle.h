//
// Created by vladi on 22.10.2022.
//

#ifndef SEFLQUESTROOM2_RAMPUZZLE_H
#define SEFLQUESTROOM2_RAMPUZZLE_H
#include "sefl_quest_core.h"
namespace SEFL {

    class RAMPuzzle: public Quest_Basic_Client {
        uint8_t * sensor_pins;
        int8_t sensor_pins_size;
        bool state;
        const bool UNTRIGGERED=false;
        const bool TRIGGERED=true;

    public:
        void setSensorPins(const uint8_t *sensorPins,int8_t sensorPinsSize);
        void setLedPins(const uint8_t *ledPins,int8_t ledPinsSize);
    private:
        uint8_t *led_pins;
        int8_t led_pins_size;
        void resetLeds();
        void setLeds();
        void updateLeds();
        bool checkResults();
    public:
        explicit RAMPuzzle(MQTTClient &mqtt, const char *name = DEFAULT_DEVICE_NAME,
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

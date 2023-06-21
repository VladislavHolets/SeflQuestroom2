//
// Created by vladi on 13.02.2023.
//

#ifndef SEFLQUESTROOM2_FLOORPUZZLE_H
#define SEFLQUESTROOM2_FLOORPUZZLE_H

#include "sefl_quest_core.h"
#include "ArduinoJson.h"
#include "Adafruit_NeoPixel.h"

namespace SEFL {

    struct Panel {
        uint8_t sensor_pin;
        uint8_t led_pin;
        int8_t press_order;
        int8_t pressed_number;
    };

    class FloorPuzzle : public Quest_Basic_Client {

        Panel *panels_array;
        int8_t panels_array_size;
        uint8_t panels_power_pin;
        enum PuzzleState {
            INITIAL,
            CORRECT_ORDER,
            INCORRECT_ORDER
        } puzzle_state;

        bool animation_start_flag;

        int8_t current_press_number;

        uint32_t animation_timestamp,
                pause_timestamp,
                booting_timestamp,
                animation_pattern_timeout,
                animation_correct_timeout,
                animation_incorrect_timeout,
                animation_pause_timeout,
                booting_up_timeout,
                idle_timestamp;
        bool ready_for_play;
        uint8_t pattern_size;
        uint8_t rounds;
        uint8_t current_round;

        Adafruit_NeoPixel strip;
        int8_t strip_size;
        uint8_t strip_pin;

        void scan_panels(bool trigger_LED = false);

        void check_panels();

        void init_puzzle();

        void show_pattern();

        void show_failure();

        void show_success();

        void show_pause();

        void refresh_state();

        void refreshStrip();
    public:

        FloorPuzzle(MQTTClient &mqtt, const char *name, uint8_t resetStatus, const char *placement,
                    const char *inTopic = DEFAUT_IN_TOPIC, const char *outTopic = DEFAUT_OUT_TOPIC,
                    Language language = ENG);

        void setPanels(Panel *panelsArray, int8_t panelsArraySize, uint8_t power_pin);

        void setPatternSize(uint8_t patternSize);

        void setStripPin(uint8_t stripPin);

        void setStripSize(int8_t stripSize);

        void onActive() override;

        void onDefault() override;

        void onFinished() override;

        void onManualFinished() override;

    };

} // SEFL

#endif //SEFLQUESTROOM2_FLOORPUZZLE_H

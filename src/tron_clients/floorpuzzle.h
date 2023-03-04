//
// Created by vladi on 13.02.2023.
//

#ifndef SEFLQUESTROOM2_FLOORPUZZLE_H
#define SEFLQUESTROOM2_FLOORPUZZLE_H
#include "sefl_quest_core.h"
namespace SEFL {

    class FloorPuzzle: public Quest_Basic_Client {
        struct Panel{
            uint8_t sensor_pin;
            uint8_t led_pin;
            int8_t press_order;
            int8_t pressed_number;
        }*panels_array;
        int8_t panels_array_size;
        enum PuzzleState{
            INITIAL,
            CORRECT_ORDER,
            INCORRECT_ORDER
        }puzzle_state;
        bool animation_start_flag;
        int8_t current_press_number;

        uint32_t animation_timestamp,
        pause_timestamp,
        animation_pattern_timeout,
        animation_correct_timeout,
        animation_incorrect_timeout,
        animation_pause_timeout;
        uint8_t pattern_size;
        void scan_panels(bool trigger_LED=false);
        void check_panels();
        void init_puzzle(uint8_t patternSize=0);
        void show_pattern();
        void show_failure();
        void show_success();
        void show_pause();
    public:

        FloorPuzzle(MQTTClient &mqtt, const char *name, uint8_t resetStatus, const char *placement, const char *inTopic=DEFAUT_IN_TOPIC, const char *outTopic=DEFAUT_OUT_TOPIC,
                    Language language=ENG);

        void setPanels(Panel *panelsArray, int8_t panelsArraySize);

        void onActive() override;

        void onDefault() override;

        void onFinished() override;

        void onManualFinished() override;

    };

} // SEFL

#endif //SEFLQUESTROOM2_FLOORPUZZLE_H

//
// Created by vladi on 25.10.2022.
//

#ifndef SEFLQUESTROOM2_GAMMAPUZZLE_H
#define SEFLQUESTROOM2_GAMMAPUZZLE_H

#include "sefl_quest_core.h"

namespace SEFL {

    class GammaPuzzle: public Quest_Basic_Client{
        enum ButtonState{
            RESETED,
            PRESSED,
            RELEASED
        };
        enum PuzzleState{
            INITIAL,
            CORRECT_ORDER,
            INCORRECT_ORDER
        }puzzle_state;
    private:
        uint8_t *buttons_states;
        uint8_t *buttons_pins;
        int8_t buttons_pins_size;
        uint8_t *neons_pins;
        int8_t neons_pins_size;
        int8_t *correct_order;
        int8_t *current_order;
        int8_t order_size;
        int8_t current_input;
        uint32_t incorrect_animation_timestamp;
        uint32_t incorrect_animation_timeout;
        uint32_t correct_animation_timestamp;
        uint32_t correct_animation_timeout;
    public:
        void setIncorrectAnimationTimeout(uint32_t incorrectAnimationTimeout);
        void setCorrectAnimationTimeout(uint32_t correctAnimationTimeout);
        void setButtonsPins(const uint8_t *buttonsPins, int8_t buttonsPinsSize);
        void setNeonsPins(const uint8_t *neonsPins, int8_t neonsPinsSize);
        void setCorrectOrder(const int8_t *correctOrder, int8_t orderSize);
        void scanButtons();
        bool checkButtons();
        void refreshNeon();
    public:
        GammaPuzzle(MQTTClient &mqtt, const char *name, uint8_t resetStatus=1, const char *placement=DEFAULT_PLACEMENT,
                    const char *inTopic=DEFAUT_IN_TOPIC, const char *outTopic=DEFAUT_OUT_TOPIC, Language language=ENG);

        void onActive() override;

        void onDefault() override;

        void onFinished() override;

        void onManualFinished() override;
    };

} // SEFL

#endif //SEFLQUESTROOM2_GAMMAPUZZLE_H

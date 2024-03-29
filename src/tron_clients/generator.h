//
// Created by vladi on 01.11.2022.
//

#ifndef SEFLQUESTROOM2_GENERATOR_H
#define SEFLQUESTROOM2_GENERATOR_H
#include <sefl_quest_core.h>
namespace SEFL {

    class Generator:public Quest_Basic_Client{
        uint8_t *wires_pins;
        int16_t *generator_motors_pins;
        bool *correct_pattern;
        bool *current_pattern;
        int16_t wires_pins_size;
        int16_t motors_pins_size;
        int16_t frame_light_pin;
        int16_t generator_leds_pin;
        int16_t motors_enable_pin;
        int16_t locker_magnet_pin;
        int16_t locker_light_pin;
    public:
        void setMotorsEnablePin(int16_t motorsEnablePin);

    private:
        uint32_t animation_timestamp;
        uint32_t correct_animation_timeout;
        uint32_t solved_animation_timeout;
        uint32_t second_motor_start_time;
        uint32_t last_motor_start_time;
        enum PuzzleStatus{
            INITIAL,CORRECT_INSERTION,SOLVING,SOLVED
        }puzzle_status;
        void scan_inputs();
        void refresh_outputs();
    public:
        void setFrameLightPin(uint8_t frameLightPin);

        void setGeneratorMotorsPin(const int16_t * generatorMotorsPins, int16_t size);

        void setGeneratorLedsPin(uint8_t generatorLedsPin);

        void setCorrectAnimationTimeout(uint32_t correctAnimationTimeout);

        void setSolvedAnimationTimeout(uint32_t solvedAnimationTimeout);

        void setWiresPins(const uint8_t *wiresPins,uint8_t size);

        void setCorrectPattern(const uint8_t *correctPattern,uint8_t size);

        Generator(MQTTClient &mqtt, const char *name, uint8_t resetStatus,
                  const char *placement, const char *inTopic=DEFAUT_IN_TOPIC, const char *outTopic=DEFAUT_OUT_TOPIC,
                  Language language=ENG);

        void onActive() override;

        void onDefault() override;

        void onFinished() override;

        void onManualFinished() override;
    };

} // SEFL

#endif //SEFLQUESTROOM2_GENERATOR_H

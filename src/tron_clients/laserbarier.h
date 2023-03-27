//
// Created by vladi on 18.02.2023.
//

#ifndef SEFLQUESTROOM2_LASERBARIER_H
#define SEFLQUESTROOM2_LASERBARIER_H
#include "sefl_quest_core.h"
#include "ArduinoJson.h"
namespace SEFL {

    struct ColorPins {
        uint8_t red;
        uint8_t blue;
        uint8_t green;
    };

    struct Barrier {
        uint8_t sensor_pin;
        uint8_t order_number;
        int crossed_order;
    };

    class LaserBarier: public Quest_Basic_Client{
        enum LaserState{
            RESETED,
            PRESSED,
            RELEASED
        };

        enum PuzzleState{
            INITIAL,
            CORRECT_ORDER,
            INCORRECT_ORDER,
            VIRUS
        }puzzle_state;

    private:

        //LED
        uint8_t *segments_pins;
        uint8_t segments_size;

        ColorPins color_pins;

        //Barrier
        Barrier *barrier;
        uint8_t barrier_size;
        uint8_t lasers_pin;

        uint8_t curr_cross;

        uint8_t code_sign_pin;
    public:
        void setCodeSignPin(uint8_t codeSignPin);

    private:

        //animation timestamps
        uint32_t animation_timestamp;
        uint32_t pause_timestamp;
        uint32_t incorrect_animation_timeout;
        uint32_t correct_animation_timeout;
        uint32_t finished_puzzle_timeout;
        uint32_t pause_timeout;

        void init();
        void refresh_puzzle();
        void show_success();
        void show_failure();
        void read_barrier();
        void clearCurrentOrder();
        void show_virus();
        void check_data();
        void clear_changes();

    public:
        void setIncorrectAnimationTimeout(uint32_t incorrectAnimationTimeout);
        void setCorrectAnimationTimeout(uint32_t correctAnimationTimeout);
        void setSegments(const uint8_t *segments, uint8_t segmentsSize);
        void setColor(ColorPins colorPins);
        void setBarrier(const uint8_t *sensors, uint8_t lasersPin, uint8_t barrierSize);
        void setOrder(const uint8_t *order, uint8_t orderSize);

        LaserBarier(MQTTClient &mqtt, const char *name, uint8_t resetStatus=1, const char *placement=DEFAULT_PLACEMENT,
        const char *inTopic=DEFAUT_IN_TOPIC, const char *outTopic=DEFAUT_OUT_TOPIC, Language language=ENG);

        void onActive() override;

        void onDefault() override;

        void onFinished() override;

        void onManualFinished() override;
    };

} // SEFL

#endif //SEFLQUESTROOM2_LASERBARIER_H

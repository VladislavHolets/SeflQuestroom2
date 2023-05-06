/*
 * tronsegmenttimer.h
 *
 *  Created on: Aug 16, 2021
 *      Author: vladi
 */

#ifndef TRONSEGMENTTIMER_H_
#define TRONSEGMENTTIMER_H_
#include "sefl_quest_core.h"
#include "ArduinoJson.h"
#include <sys/_stdint.h>

namespace SEFL
{

     const uint8_t TIMER_DIGITS[16]{
            //0bABCDEFGDP
            /*  AAAAAA
             * F      B
             * F      B
             * F      B
             *  GGGGGG
             * E      C
             * E      C
             * E      C
             *  DDDDDD
             *         DP
             * */
            0b11111100,
            0b01100000,
            0b11011010,
            0b11110010,
            0b01100110,
            0b10110110,
            0b10111110,
            0b11100000,
            0b11111110,
            0b11110110,
            0b11101110,
            0b00111110,
            0b10011100,
            0b01111010,
            0b10011110,
            0b10001110,
    };
	class Tron_Segment_Timer : public Quest_Actuator_Client
	{
       HC595_Driver *driver_;
       uint8_t *segments_;
       uint8_t *segments_value_;
       uint8_t *segments_base_;
       uint32_t overflow_period_;
       int8_t segments_size_;
       int64_t start_timestamp;
       bool increasing_order;
       int16_t starting_value,stopping_value;
       int32_t offset_minutes_;
       void refresh_timer();
       bool calculate_values();
       void start_timer();
       void stop_timer();
	public:
        void setStoppingValue(int16_t stoppingValue);

        void setIncreasingOrder(bool increasingOrder);

        void setStartingValue(int16_t startingValue);

        void setSegments(const uint8_t *segments,int8_t size);
        void setSegmentsBase(const uint8_t *segmentsBase,int8_t size);
        void setOverflowPeriod(uint32_t overflowPeriod);
        void setDriver(HC595_Driver &driver);
        explicit Tron_Segment_Timer(MQTTClient &mqtt, const char *name = DEFAULT_DEVICE_NAME,
						   uint8_t reset_status = 1, const char *placement = DEFAULT_PLACEMENT,
						   const char *in_topic = DEFAUT_IN_TOPIC, const char *out_topic = DEFAUT_OUT_TOPIC, SEFL::Language language = UKR);
		~Tron_Segment_Timer() override;
        void onOn() override;
        void onOff() override;
	};

} /* namespace SEFL */

#endif /* TRONSEGMENTTIMER_H_ */

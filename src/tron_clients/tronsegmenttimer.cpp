/*
 * tronsegmenttimer.cpp
 *
 *  Created on: Aug 16, 2021
 *      Author: vladi
 */

#include "tronsegmenttimer.h"

namespace SEFL
{

	Tron_Segment_Timer::Tron_Segment_Timer(MQTTClient &mqtt,  const char *name,
										   uint8_t reset_status, const char *placement,
										   const char *in_topic, const char *out_topic, SEFL::Language language) : Quest_Actuator_Client(mqtt, name, reset_status, placement, in_topic,
																																	  out_topic, language)

	{
    driver_= nullptr;
    segments_= nullptr;
    segments_base_= nullptr;
    segments_value_= nullptr;
    start_timestamp = -1;
    segments_size_=-1;
    overflow_period_=1000;
    increasing_order=false;
    offset_minutes_=0;
    starting_value=0;
    stopping_value=0;

		// TODO Auto-generated constructor stub
	}

	Tron_Segment_Timer::~Tron_Segment_Timer()
	{
		// TODO Auto-generated destructor stub
	}



    void Tron_Segment_Timer::setDriver(HC595_Driver &driver) {
        driver_ = &driver;
    }

    void Tron_Segment_Timer::setSegments(const uint8_t *segments,int8_t size) {
        if (segments_!= nullptr){
            delete [] segments_;
            delete [] segments_base_;
            delete [] segments_value_;
        }
        segments_size_=size;
        segments_=new uint8_t [size];
        segments_base_=new uint8_t [size];
        segments_value_=new uint8_t [size];

        for (int i = 0; i < segments_size_; ++i) {
            segments_[i]=segments[i];
            segments_value_[i]=0;
            segments_base_[i]=DEC;
        }
    }

    void Tron_Segment_Timer::setSegmentsBase(const uint8_t *segmentsBase,int8_t size) {
        if (size!=segments_size_){
            return;
        }
        for (int i = 0; i < segments_size_; ++i) {
            segments_base_[i]=segmentsBase[i];
        }
    }

    void Tron_Segment_Timer::setOverflowPeriod(uint32_t overflowPeriod) {
        overflow_period_ = overflowPeriod;
    }

    void Tron_Segment_Timer::refresh_timer() {
        for (int i = 0; i < segments_size_; ++i) {
            driver_->setDataByte(segments_[i], TIMER_DIGITS[segments_value_[i]]);//setting each timer byte
        }
        driver_->sendData();
    }

    bool Tron_Segment_Timer::calculate_values() {
        if(start_timestamp<0){
            for (int i = 0; i < segments_size_; ++i) {
                segments_value_[i]=0;
            }
            return true;
        }
        static uint32_t calculated_value;
        int64_t temp=((increasing_order)?1:-1)*(millis()-start_timestamp)/overflow_period_+starting_value;
        if(temp>starting_value &&(temp>=stopping_value)){
            this->setStatus(ActuatorClientStatuses::OFF_STATUS);
        }
        if(temp<starting_value &&(temp<=stopping_value)){
            this->setStatus(ActuatorClientStatuses::OFF_STATUS);
        }
        if(calculated_value!=temp){
            calculated_value=temp+offset_minutes_*((increasing_order)?-1:1);
        }else{
            return false;
        }
        for (int i = 0; i < segments_size_; ++i) {
            segments_value_[i]=(calculated_value-(calculated_value/segments_base_[i])*segments_base_[i]);
            calculated_value/=segments_base_[i];
        }
            return true;
    }

    void Tron_Segment_Timer::start_timer() {
        start_timestamp=millis();
    }

    void Tron_Segment_Timer::stop_timer() {
        start_timestamp=-1;
    }

    void Tron_Segment_Timer::onOn() {
        if(isChangedStatus()){
            setChangedStatus(false);
            reportStatus();
            start_timer();
            offset_minutes_=0;
        }
        if(calculate_values()){
        refresh_timer();
        }
        if (!data.empty()) {
            StaticJsonDocument<SEFL::DOC_SIZE> doc;

            DeserializationError error = deserializeJson(doc, data.front());
            if (error) {
                Logger::notice(F("deserializeJson() failed: "));
                Logger::notice(error.f_str());
                return;
            }

            if (doc.containsKey("starting_value"))
                starting_value= doc["starting_value"].as<int16_t>();
            if (doc.containsKey("stopping_value"))
                stopping_value= doc["stopping_value"].as<int16_t>();
            if (doc.containsKey("increasing_order"))
                increasing_order= doc["increasing_order"].as<bool>();
            if (doc.containsKey("overflow_period"))
                overflow_period_= doc["overflow_period"].as<uint32_t>();
            if (doc.containsKey("segments_base"))
                for (int i = 0; i < segments_size_; ++i) {
                    segments_base_[i]= doc["segments_base"].as<uint8_t>();
                }
            if (doc.containsKey("offset_minutes"))
                offset_minutes_= doc["offset_minutes"].as<int32_t>();
            if (doc.containsKey("add_offset_minutes"))
                offset_minutes_+= doc["add_offset_minutes"].as<int32_t>();

            data.remove(0);
        }
    }

    void Tron_Segment_Timer::onOff() {
        if(isChangedStatus()){
            setChangedStatus(false);
            reportStatus();
            stop_timer();
            calculate_values();
            refresh_timer();
        }
    }

    void Tron_Segment_Timer::setIncreasingOrder(bool increasingOrder) {
        increasing_order = increasingOrder;
    }

    void Tron_Segment_Timer::setStartingValue(int16_t startingValue) {
        starting_value = startingValue;
    }

    void Tron_Segment_Timer::setStoppingValue(int16_t stoppingValue) {
        stopping_value = stoppingValue;
    }

} /* namespace SEFL */

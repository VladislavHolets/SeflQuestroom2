//
// Created by vladi on 13.02.2023.
//

#include "floorpuzzle.h"

namespace SEFL {
    FloorPuzzle::FloorPuzzle(MQTTClient &mqtt, const char *name, uint8_t resetStatus, const char *placement,
                             const char *inTopic, const char *outTopic, Language language) : Quest_Basic_Client(mqtt,
                                                                                                                name,
                                                                                                                resetStatus,
                                                                                                                placement,
                                                                                                                inTopic,
                                                                                                                outTopic,
                                                                                                                language) {
        panels_array = nullptr;
        panels_array_size = 0;
        puzzle_state=INITIAL;
        animation_start_flag=false;
        current_press_number=0;
        animation_timestamp=0;
        pause_timestamp=0;
        animation_pattern_timeout = 0;
        animation_correct_timeout = 0;
        animation_incorrect_timeout = 0;
        animation_pause_timeout=0;
        pattern_size=0;
        panels_power_pin=-1;
    }

    void FloorPuzzle::onActive() {
        if (this->changed_status_)
        {
            this->changed_status_ = false;
            this->reportStatus();
            init_puzzle();
        }
        scan_panels(true);
//        if(current_press_number >=0){
//            check_panels();
//        }else{
//            show_pattern();
//            scan_panels();
//        }

    }

    void FloorPuzzle::onDefault() {
        if (this->changed_status_)
        {
            this->changed_status_ = false;
            this->reportStatus();
        }
        Pext.digitalWrite(panels_power_pin, HIGH);
        for(int i = 0; i < panels_array_size; i++) {
            Pext.digitalWrite(panels_array[i].led_pin, HIGH);
        }
    }

    void FloorPuzzle::onFinished() {
        if (this->changed_status_)
        {
            this->changed_status_ = false;
            this->reportStatus();
        }
    }

    void FloorPuzzle::onManualFinished() {
        if (this->changed_status_)
        {
            this->changed_status_ = false;
            this->reportStatus();
        }
    }


    void FloorPuzzle::init_puzzle(uint8_t patternSize) {
        if(patternSize==0){
            pattern_size=panels_array_size;
        }else{
            pattern_size=patternSize;
        }
        auto *pattern_reservoir= new int8_t[pattern_size];
        for(int i=0;i<panels_array_size;i++){
            panels_array[i].press_order=-1;
            panels_array[i].pressed_number=-1;
            if(i<pattern_size) {
                pattern_reservoir[i] = i;
            }
        }
        for(int i=0;i<pattern_size;){
            int8_t r = rand() % panels_array_size + 1;
            if (panels_array[r - 1].press_order==-1) {
                panels_array[r - 1].press_order = pattern_reservoir[i++];

                i++;
            }
        }
        current_press_number=-1;
        Pext.digitalWrite(panels_power_pin, LOW);
        delete[] pattern_reservoir;
    }

    void FloorPuzzle::show_pattern() {
        if(animation_timestamp==0){
            return;
        }
        if(millis()-animation_timestamp>animation_pattern_timeout){
            for(int i=0;i<panels_array_size;i++){
                Pext.digitalWrite(panels_array[i].led_pin,false);
            }
            animation_timestamp=0;
            pause_timestamp=millis();
            return;
        }
        auto state=(millis()-animation_timestamp)/(animation_pattern_timeout/pattern_size);
        for(int i=0;i<panels_array_size;i++){
            Pext.digitalWrite(panels_array[i].led_pin,(state==panels_array[i].press_order));
        }
    }


    void FloorPuzzle::show_failure() {
        if(animation_timestamp==0){
            return;
        }
        if(millis()-animation_timestamp>animation_incorrect_timeout){
            for(int i=0;i<panels_array_size;i++){
                Pext.digitalWrite(panels_array[i].led_pin,false);
            }
            animation_timestamp=0;
            init_puzzle();
            pause_timestamp=millis();
            return;
        }
//        auto state=(millis()-animation_timestamp)/(animation_pattern_timeout/pattern_size);
//        for(int i=0;i<panels_array_size;i++){
//            Pext.digitalWrite(panels_array[i].led_pin,(state==panels_array[i].press_order));
//        }

    }

    void FloorPuzzle::show_success() {
        if(animation_timestamp==0){
            return;
        }
        if(millis()-animation_timestamp>animation_correct_timeout){
            for(int i=0;i<panels_array_size;i++){
                Pext.digitalWrite(panels_array[i].led_pin,false);
            }
            animation_timestamp=0;
            this->setStatus(FINISHED_STATUS);
            pause_timestamp=millis();
            return;
        }
//        auto state=(millis()-animation_timestamp)/(animation_pattern_timeout/pattern_size);
//        for(int i=0;i<panels_array_size;i++){
//            Pext.digitalWrite(panels_array[i].led_pin,(state==panels_array[i].press_order));
//        }
    }

    void FloorPuzzle::show_pause() {
        if(pause_timestamp==0){
            return;
        }
        if(millis()-pause_timestamp>animation_pause_timeout){
            pause_timestamp=0;
            return;
        }
        for(int i=0;i<panels_array_size;i++){
            Pext.digitalWrite(panels_array[i].led_pin,false);
        }
    }

    void FloorPuzzle::setPanels(Panel *panelsArray, int8_t panelsArraySize, uint8_t power_pin) {
        if (panels_array!= nullptr){
            delete [] panels_array;
        }
        panels_power_pin=power_pin;
        panels_array_size=panelsArraySize;
        panels_array=new Panel[panels_array_size];
        for(int i=0;i<panels_array_size;i++){
            panels_array[i].led_pin=panelsArray[i].led_pin;
            panels_array[i].sensor_pin=panelsArray[i].sensor_pin;
            panels_array[i].press_order=panelsArray[i].press_order;
            panels_array[i].pressed_number=0;
        }
    }

    void FloorPuzzle::scan_panels(bool trigger_LED) {
        pinMode(Mext.getCi(),INPUT_PULLUP);
        for(int i=0;i<panels_array_size;i++){
            bool temp=Mext.digitalRead(panels_array[i].sensor_pin);
            if (!temp && panels_array[i].pressed_number!=(current_press_number-1)){
                //react on press
                panels_array[i].pressed_number=current_press_number++;
            }
            if(trigger_LED){
                Pext.digitalWrite(panels_array[i].led_pin,!temp);
            }

        }
    }

    void FloorPuzzle::check_panels() {
        if(current_press_number>=pattern_size){
            for(int i=0;i<panels_array_size;i++){
                if(panels_array[i].press_order!=panels_array[i].pressed_number){
                    puzzle_state=INCORRECT_ORDER;
                    animation_timestamp=millis();
                    return;
                }
            }
            puzzle_state=CORRECT_ORDER;
            animation_timestamp=millis();
        }
        scan_panels(true);
        puzzle_state=INITIAL;
    }
} // SEFL
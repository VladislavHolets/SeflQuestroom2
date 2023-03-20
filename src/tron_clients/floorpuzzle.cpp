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
        current_press_number=-1;
        animation_timestamp=0;
        pause_timestamp=0;
        animation_pattern_timeout = 0;
        animation_correct_timeout = 3000;
        animation_incorrect_timeout = 3000;
        animation_pause_timeout= 0;
        pattern_size=0;
        panels_power_pin=-1;
        rounds = 1;
    }

    void FloorPuzzle::onActive() {
        if (this->changed_status_)
        {
            this->changed_status_ = false;
            this->reportStatus();
            animation_timestamp = millis();
            Pext.digitalWrite(panels_power_pin, LOW);
            puzzle_state=INITIAL;
        }
        refresh_state();
    }

    void FloorPuzzle::onDefault() {
        if (this->changed_status_)
        {
            this->changed_status_ = false;
            this->reportStatus();
            init_puzzle();
        }
        Pext.digitalWrite(panels_power_pin, HIGH);
        for(int i = 0; i < panels_array_size; i++) {
            Pext.digitalWrite(panels_array[i].led_pin, HIGH);
        }
        if (!data.empty()) {
            StaticJsonDocument<SEFL::DOC_SIZE> doc;

            DeserializationError error = deserializeJson(doc, data.front());
            if (error) {
                Logger::notice(F("deserializeJson() failed: "));
                Logger::notice(error.f_str());
                return;
            }
            JsonInteger players = doc["floor"];
            if(players == 2 || players == 3) {
                rounds = 3;
                pattern_size = players;
            }
            else if (players == 4 || players == 5) {
                rounds = 2;
                pattern_size = players;
            }

            data.remove(0);
        }
    }

    void FloorPuzzle::onFinished() {
        if (this->changed_status_)
        {
            this->changed_status_ = false;
            this->reportStatus();
            animation_timestamp = millis();
        }
        show_success();

    }

    void FloorPuzzle::onManualFinished() {
        if (this->changed_status_)
        {
            this->changed_status_ = false;
            this->reportStatus();
            animation_timestamp = millis();
        }
        show_success();
//        delay(300);
    }

    void FloorPuzzle::refresh_state() {
        switch(puzzle_state) {
            case INITIAL: {
                if(current_press_number >= pattern_size){
                    check_panels();
                }else{
                    show_pattern();
                }
                break;
            }
            case CORRECT_ORDER:{
                show_success();
                if(pause_timestamp != 0) {
                    show_pause();
                }
                SEFL::Logger::notice("floor_puzzle", "Correct");
                break;
            }
            case INCORRECT_ORDER:{
                show_failure();
                if (pause_timestamp != 0) {
                    show_pause();
                }
                SEFL::Logger::notice("floor_puzzle", "Incorrect");
                break;
            }
        }
    }

    void FloorPuzzle::init_puzzle() {
        auto *pattern_reservoir= new int8_t[pattern_size];
        for(int i=0;i<panels_array_size;i++){
            panels_array[i].press_order=-1;
            panels_array[i].pressed_number=-1;
            if(i<pattern_size) {
                pattern_reservoir[i] = i;
            }
        }

        int i = 0;
        while( i < pattern_size){
            int8_t r = rand() % panels_array_size + 1;
            if (panels_array[r - 1].press_order == -1) {
                panels_array[r - 1].press_order = pattern_reservoir[i];

                i++;
            }
        }
        current_press_number = 0;
        Pext.digitalWrite(panels_power_pin, LOW);
        pinMode(Mext.getCi(),INPUT_PULLUP);

        animation_pattern_timeout = 1000 * pattern_size;

        delete[] pattern_reservoir;
    }

    void FloorPuzzle::show_pattern() {
        if(animation_timestamp==0){
            scan_panels(true);
            return;
        }
        if(millis()-animation_timestamp>animation_pattern_timeout){
            for(int i=0;i<panels_array_size;i++){
                Pext.digitalWrite(panels_array[i].led_pin,true);
            }
            animation_timestamp=0;
            pause_timestamp=millis();
            return;
        }
        auto state=(millis()-animation_timestamp)/(animation_pattern_timeout/pattern_size);
        for(int i=0;i<panels_array_size;i++){

            Pext.digitalWrite(panels_array[i].led_pin,!(state>=panels_array[i].press_order));
        }
    }


    void FloorPuzzle::show_failure() {
        if(animation_timestamp==0){
            puzzle_state=INITIAL;
            current_press_number=0;
            animation_timestamp = millis();
            for(int i = 0; i< panels_array_size; i++) {
                panels_array[i].pressed_number=-1;
            }
            return;
        }
        if(millis()-animation_timestamp>animation_incorrect_timeout){
            for(int i=0;i<panels_array_size;i++){
                Pext.digitalWrite(panels_array[i].led_pin,false);
            }
            animation_timestamp=0;
            pause_timestamp=millis();
            return;
        }
        //auto state=(millis()-animation_timestamp)/(animation_pattern_timeout/pattern_size);
        for(int i=0;i<panels_array_size;i++){
            Pext.digitalWrite(panels_array[i].led_pin,(millis()-animation_timestamp)/300%2);
        }

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
            pause_timestamp=millis();
            return;
        }
        //auto state=(millis()-animation_timestamp)/(animation_pattern_timeout/pattern_size);
        for(int i=0;i<panels_array_size;i++){
            Pext.digitalWrite(panels_array[i].led_pin, LOW);
        }
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
            Pext.digitalWrite(panels_array[i].led_pin,true);
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
        for(int i=0; i < panels_array_size;i++){
            bool temp=Mext.digitalRead(panels_array[i].sensor_pin);
//            if (temp) {
            if (temp && (panels_array[i].pressed_number == -1) ){
                //react on press
                panels_array[i].pressed_number = current_press_number;
                current_press_number++;
                SEFL::Logger::notice("floor_puzzle", i);
            }
            if(trigger_LED){
                Pext.digitalWrite(panels_array[i].led_pin,!temp);
            }

        }
    }

    void FloorPuzzle::check_panels() {
        if(current_press_number>=pattern_size){
            for(int i=0;i<panels_array_size;i++){
                bool curr_state = Mext.digitalRead(panels_array[i].sensor_pin);
                if((!curr_state && panels_array[i].pressed_number != -1)
                || panels_array[i].press_order!=panels_array[i].pressed_number){
                    puzzle_state=INCORRECT_ORDER;
                    animation_timestamp=millis();
                    return;
                }
            }
            puzzle_state=CORRECT_ORDER;
            this->setStatus(FINISHED_STATUS);
            animation_timestamp=millis();
        }
//        scan_panels(true);
//        puzzle_state=INITIAL;
    }

    void FloorPuzzle::setPatternSize(uint8_t patternSize) {
        pattern_size = patternSize;
    }
} // SEFL
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
        animation_start_flag=true;
        current_press_number=-1;
        animation_timestamp=0;
        pause_timestamp=0;
        animation_pattern_timeout = 0;
        animation_correct_timeout = 3000;
        animation_incorrect_timeout = 3000;
        animation_pause_timeout= 1500;
        pattern_size=2;
        panels_power_pin=-1;
        rounds = 5;
        current_round = 1;
        booting_up_timeout = 3000;
        this->strip.setPin(Mext.getCi());
        this->strip.updateType(NEO_GRB + NEO_KHZ800);
    }

    void FloorPuzzle::onActive() {
        if (this->changed_status_)
        {
            this->changed_status_ = false;
            this->reportStatus();
            animation_timestamp = millis();
            Pext.digitalWrite(panels_power_pin, LOW);
            puzzle_state=INITIAL;
            booting_timestamp=millis();
            pinMode(Mext.getCi(),INPUT);
            Mext.digitalRead(strip_pin);
            strip.begin();
            strip.clear();
            strip.show();
            strip.show();
        }
        if (millis()-booting_timestamp>booting_up_timeout) {
            refresh_state();
        }
refreshStrip();
    }

    void FloorPuzzle::onDefault() {
        if (this->changed_status_)
        {
            this->changed_status_ = false;
            this->reportStatus();
            init_puzzle();
        }

        refreshStrip();
        Pext.digitalWrite(panels_power_pin, HIGH);
        for(int i = 0; i < panels_array_size; i++) {
            Pext.digitalWrite(panels_array[i].led_pin, HIGH);
        }

        current_round = 1;

        if (!data.empty()) {
            StaticJsonDocument<SEFL::DOC_SIZE> doc;

            DeserializationError error = deserializeJson(doc, data.front());
            if (error) {
                Logger::notice(F("deserializeJson() failed: "));
                Logger::notice(error.f_str());
                return;
            }

            if (doc["floor_rounds"].as<uint8_t>() != 0)
                rounds = doc["floor_rounds"].as<uint8_t>();

            if (doc["floor_players"].as<uint8_t>() != 0)
                pattern_size = doc["floor_players"].as<uint8_t>();

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

        refreshStrip();
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
                    scan_panels(!animation_start_flag);
                    if (animation_start_flag)
                        show_pattern();

                    if(millis()-idle_timestamp>3000){
                        animation_start_flag=true;
                        for(int i=0;i<panels_array_size;i++){
                            panels_array[i].pressed_number=-1;
                            current_press_number=0;
                        }
                    }
                }
                break;
            }
            case CORRECT_ORDER:{
                show_success();
                if(pause_timestamp != 0) {
                    show_pause();
                }
                break;
            }
            case INCORRECT_ORDER:{
                show_failure();
                if (pause_timestamp != 0) {
                    show_pause();
                }
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
        animation_start_flag = true;

        delete[] pattern_reservoir;
    }

    void FloorPuzzle::show_pattern() {
        if(animation_timestamp==0){
            show_pause();
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

            Pext.digitalWrite(panels_array[i].led_pin, state < panels_array[i].press_order);
        }
    }


    void FloorPuzzle::show_failure() {
        scan_panels(false);
        if(animation_timestamp==0 && ready_for_play){
            puzzle_state=INITIAL;
            current_press_number=0;
            current_round=1;
            animation_timestamp = millis();
            for(int i = 0; i< panels_array_size; i++) {
                panels_array[i].pressed_number=-1;
            }
            strip.clear();
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
        scan_panels(false);
        if(animation_timestamp==0 && ready_for_play){
            if (current_round < rounds ) {
                puzzle_state = INITIAL;
                animation_timestamp=millis();
                current_round++;
                init_puzzle();
            } else {
                this->setStatus(FINISHED_STATUS);
            }
            return;
        }
        if(millis()-animation_timestamp>animation_correct_timeout){
            for(int i=0;i<panels_array_size;i++){
                Pext.digitalWrite(panels_array[i].led_pin,HIGH);
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
            animation_timestamp=millis();
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
            panels_array[i].pressed_number=-1;
        }
    }

    void FloorPuzzle::scan_panels(bool trigger_LED) {
        bool panels_released=true;
        pinMode(Mext.getCi(), INPUT_PULLUP);
        for(int i=0; i < panels_array_size;i++){
            bool temp=Mext.digitalRead(panels_array[i].sensor_pin);
//            if (temp) {
            if(temp) {
                idle_timestamp = millis();
            }
            if (temp && (panels_array[i].pressed_number == -1) ){
                //react on press
                panels_array[i].pressed_number = current_press_number;
                current_press_number++;
                animation_start_flag = false;
                //ready_for_play=true;
                //SEFL::Logger::notice("floor_puzzle", i);

            }
            if(temp){
                panels_released=false;
            }
            if(trigger_LED){
                Pext.digitalWrite(panels_array[i].led_pin,!temp);
            }

        }
        if(panels_released){
            ready_for_play=true;
        }

    }

    void FloorPuzzle::check_panels() {
        if(!ready_for_play){return;}
        if(current_press_number>=pattern_size){
            for(int i=0;i<panels_array_size;i++){
                Mext.digitalRead(panels_array[i].sensor_pin);
                pinMode(Mext.getCi(), INPUT_PULLUP);
                bool curr_state = Mext.digitalRead(panels_array[i].sensor_pin);
                if((!curr_state && panels_array[i].pressed_number != -1)
                || panels_array[i].press_order!=panels_array[i].pressed_number){
                    puzzle_state=INCORRECT_ORDER;
                    animation_timestamp = millis();
                    animation_start_flag = true;
                    ready_for_play=false;
                    return;
                }
            }
            puzzle_state=CORRECT_ORDER;
            animation_timestamp=millis();
            animation_start_flag = true;
            ready_for_play=false;

        }
//        scan_panels(true);
//        puzzle_state=INITIAL;
    }

    void FloorPuzzle::setPatternSize(uint8_t patternSize) {
        pattern_size = patternSize;
    }

    void FloorPuzzle::setStripPin(uint8_t stripPin) {
        if (stripPin >= 0 && stripPin < 16)
        {
            this->strip_pin = stripPin;
        }
    }

    void FloorPuzzle::refreshStrip() {
        Mext.digitalRead(this->strip_pin);
        pinMode(Mext.getCi(),INPUT);
        //Logger::notice(this->getName(),"BEFORE STRIP BEGIN");
        strip.begin();

        if(puzzle_state==INITIAL){
            //strip.clear();
        }else if(puzzle_state==CORRECT_ORDER){
            strip.clear();
            strip.fill(Adafruit_NeoPixel::Color(0, 255, 0),0,strip_size*current_round/rounds);
        }else if(puzzle_state==INCORRECT_ORDER){
            strip.clear();
            strip.fill(((millis()/500)%2)?Adafruit_NeoPixel::Color(255,0 , 0):Adafruit_NeoPixel::Color(0,0 , 0),0,strip_size);
        }

        this->strip.show();
        this->strip.show();
    }

    void FloorPuzzle::setStripSize(int8_t stripSize) {
        strip_size = stripSize;
        this->strip.updateLength(strip_size);
    }
} // SEFL
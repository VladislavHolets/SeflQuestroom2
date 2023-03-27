//
// Created by vladi on 25.10.2022.
//

#include "gammapuzzle.h"

namespace SEFL {
    GammaPuzzle::GammaPuzzle(MQTTClient &mqtt, const char *name, uint8_t resetStatus, const char *placement,
                             const char *inTopic, const char *outTopic, Language language) : Quest_Basic_Client(mqtt,
                                                                                                                name,
                                                                                                                resetStatus,
                                                                                                                placement,
                                                                                                                inTopic,
                                                                                                                outTopic,
                                                                                                                language) {
        buttons_states= nullptr;
        buttons_pins= nullptr;
        neons_pins= nullptr;
        correct_order= nullptr;
        current_order= nullptr;
        buttons_pins_size=-1;
        neons_pins_size=-1;
        current_input=0;
        puzzle_state=INITIAL;
        incorrect_animation_timeout=0;
        incorrect_animation_timestamp=0;
        correct_animation_timeout=0;
        correct_animation_timestamp=0;
        refresh_neon_flag=true;
    }

    void GammaPuzzle::setButtonsPins(const uint8_t *buttonsPins,int8_t buttonsPinsSize) {
        if(buttons_pins!= nullptr){
            delete [] buttons_pins;
            delete [] buttons_states;
        }
        buttons_pins_size=buttonsPinsSize;
        buttons_pins=new uint8_t [buttons_pins_size];
        buttons_states=new uint8_t [buttons_pins_size];
        for (int i = 0; i < buttons_pins_size; ++i) {
            buttons_pins[i]=buttonsPins[i];
            buttons_states[i]=RESETED;
        }
    }

    void GammaPuzzle::setNeonsPins(const uint8_t *neonsPins,int8_t neonsPinsSize) {
        if(neons_pins!= nullptr){
            delete [] neons_pins;
        }
        neons_pins_size=neonsPinsSize;
        neons_pins=new uint8_t [neons_pins_size];
        for (int i = 0; i < neons_pins_size; ++i) {
            neons_pins[i]=neonsPins[i];
        }
    }

    void GammaPuzzle::setCorrectOrder(const int8_t *correctOrder,int8_t orderSize) {
        if(correct_order!= nullptr){
            delete [] correct_order;
        }
        if(current_order!= nullptr){
            delete [] current_order;
        }
        order_size=orderSize;
        correct_order=new int8_t [order_size];
        current_order=new int8_t [order_size];
        for (int i = 0; i < order_size; ++i) {
            correct_order[i]=correctOrder[i];
            current_order[i]=-1;
        }
    }

    void GammaPuzzle::onActive() {
        if (this->changed_status_)
        {
            this->changed_status_ = false;
            this->reportStatus();
            puzzle_state=INITIAL;
            current_input=0;
            refresh_neon_flag=true;
            refreshNeon();
        }
        scanButtons();

        refreshNeon();
        if(checkButtons()){
            current_input=0;
            refreshNeon();
            this->setStatus(BasicClientStatuses::FINISHED_STATUS);
        }

        Logger::notice(this->getName(),String("current input is  ")+String(current_input));

        Logger::notice(this->getName(),String("puzzle_state is  ")+String(puzzle_state));

        //Logger::notice(this->getName(),String(" is  ")+String(current_input));

        // Logger::notice(this->getName(),String("current input is  ")+String(current_input));
    }

    void GammaPuzzle::onDefault() {
        if (this->changed_status_)
        {
            this->changed_status_ = false;
            this->reportStatus();
            current_input=0;
            puzzle_state=INITIAL;
            refresh_neon_flag=true;
            refreshNeon();
        }
    }

    void GammaPuzzle::onFinished() {
        if (this->changed_status_)
        {
            this->changed_status_ = false;
            this->reportStatus();
        }
        refreshNeon();
    }

    void GammaPuzzle::onManualFinished() {
        if (this->changed_status_)
        {
            this->changed_status_ = false;
            this->reportStatus();
            puzzle_state=CORRECT_ORDER;
            correct_animation_timestamp=millis();
            refresh_neon_flag=true;
        }
        onFinished();
    }

    void GammaPuzzle::scanButtons() {
        if(puzzle_state!=INITIAL){
            return;
        }//CORRECT OR INCORRECT ANIMATIONS ARE NOT HAVING INPUT
        pinMode(Mext.getCi(),INPUT_PULLUP);
        for (int8_t i = 0; i < this->buttons_pins_size; i++)
        {
            bool value = Mext.digitalRead(this->buttons_pins[i]);
            if (value == LOW && this->buttons_states[i] == ButtonState::RESETED)
            {
                SEFL::Logger::notice(this->getName(), buttons_pins[i]);
                SEFL::Logger::notice(this->getName(), value);
                this->buttons_states[i] = ButtonState::PRESSED;
            }
            else if (value == HIGH && this->buttons_states[i] == ButtonState::PRESSED)
            {
                this->buttons_states[i] = ButtonState::RELEASED;
                if(current_input>0){
                    if(this->current_order[current_input-1]!=i){
                        this->current_order[current_input]=i;
                        current_input++;
                        refresh_neon_flag=true;
                    }
                }
                else{
                    this->current_order[current_input]=i;
                    current_input++;
                    refresh_neon_flag=true;

                }
                this->buttons_states[i] = ButtonState::RESETED;
            }
        }
    }

    bool GammaPuzzle::checkButtons() {
        if (current_input<=0){
            puzzle_state=INITIAL;
            return false;
        }
        for (int8_t i = 0; i < current_input; ++i) {
            if(current_order[i]!=correct_order[i] && puzzle_state!=INCORRECT_ORDER){
                puzzle_state=INCORRECT_ORDER;
                incorrect_animation_timestamp=millis();
                refresh_neon_flag=true;
                return false;}
        }
        if (current_input==order_size){
            puzzle_state=CORRECT_ORDER;
            correct_animation_timestamp=millis();
            refresh_neon_flag=true;
            return true;
        }
        return false;
    }

    void GammaPuzzle::setIncorrectAnimationTimeout(uint32_t incorrectAnimationTimeout) {
        incorrect_animation_timeout = incorrectAnimationTimeout;
    }

    void GammaPuzzle::setCorrectAnimationTimeout(uint32_t correctAnimationTimeout) {
        correct_animation_timeout = correctAnimationTimeout;
    }

    void GammaPuzzle::refreshNeon() {
        if (!refresh_neon_flag)return;
        if(puzzle_state==INITIAL){
            for (int i = 0; i < neons_pins_size; ++i) {
                Pext.digitalWrite(neons_pins[i],HIGH);
            }
            for (int i = 0; i < current_input; ++i) {
                Pext.digitalWrite(neons_pins[current_order[i]],(i<current_input)?LOW:HIGH);
            }
//            for (int i = current_input; i < neons_pins_size; ++i) {
//                Pext.digitalWrite(neons_pins[i],HIGH);
//            }
            refresh_neon_flag=false;
        }else if (puzzle_state==INCORRECT_ORDER){
            for (int i = 0; i < neons_pins_size; ++i) {
                Pext.digitalWrite(neons_pins[i],HIGH);

            }
            if((millis()-incorrect_animation_timestamp)<incorrect_animation_timeout){
                for (int i = 0; i < current_input; ++i) {
                    Pext.digitalWrite(neons_pins[current_order[i]],((millis()-incorrect_animation_timestamp)/500%2)?HIGH:LOW);

                }

            }else{
                for (int i = 0; i < order_size; ++i) {
                    Pext.digitalWrite(neons_pins[correct_order[i]],HIGH);

                }
                current_input=0;
                puzzle_state=INITIAL;
                refresh_neon_flag=false;
            }
        }else if(puzzle_state==CORRECT_ORDER){
            if((millis()-correct_animation_timestamp)<correct_animation_timeout){
                for (int i = 0; i < order_size; ++i) {
                    Pext.digitalWrite(neons_pins[correct_order[i]],(((millis()-correct_animation_timestamp)/250+i)%2)?HIGH:LOW);
                }
            }else{
                for (int i = 0; i < order_size; ++i) {
                    Pext.digitalWrite(neons_pins[i],LOW);
                }
                puzzle_state=INITIAL;
                refresh_neon_flag=false;
            }
        }
    }


} // SEFL
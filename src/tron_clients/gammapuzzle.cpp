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
        buttons_pins_size=-1;
        neons_pins_size=-1;
        current_input=0;
        puzzle_state=INITIAL;
        incorrect_animation_timeout=0;
        incorrect_animation_timestamp=0;
        correct_animation_timeout=0;
        correct_animation_timestamp=0;
    }

    void GammaPuzzle::setButtonsPins(const uint8_t *buttonsPins,int8_t buttonsPinsSize) {
        if(buttons_pins!= nullptr){
            delete buttons_pins;
            delete buttons_states;
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
            delete neons_pins;
        }
        neons_pins_size=neonsPinsSize;
        neons_pins=new uint8_t [neons_pins_size];
        for (int i = 0; i < neons_pins_size; ++i) {
            neons_pins[i]=neonsPins[i];
        }
    }

    void GammaPuzzle::setCorrectOrder(const int8_t *correctOrder,int8_t orderSize) {
        if(correct_order!= nullptr){
            delete correct_order;
        }
        if(current_order!= nullptr){
            delete current_order;
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
            refreshNeon();
        }
        scanButtons();
        if(checkButtons()){
            this->setStatus(BasicClientStatuses::FINISHED_STATUS);
        }
    }

    void GammaPuzzle::onDefault() {
        if (this->changed_status_)
        {
            this->changed_status_ = false;
            this->reportStatus();
        }
    }

    void GammaPuzzle::onFinished() {
        if (this->changed_status_)
        {
            this->changed_status_ = false;
            this->reportStatus();
        }
    }

    void GammaPuzzle::onManualFinished() {
        if (this->changed_status_)
        {
            this->changed_status_ = false;
            this->reportStatus();
        }
    }

    void GammaPuzzle::scanButtons() {
        if(puzzle_state!=INITIAL){
            return;
        }//CORRECT OR INCORRECT ANIMATIONS ARE NOT HAVING INPUT
        pinMode(Mext.getCi(),INPUT_PULLUP);
        for (int8_t i = 0; i < this->buttons_pins_size; i++)
        {

            bool value = Mext.digitalRead(this->buttons_pins[i]);

            // SEFL::Logger::notice(this->getName(), buttons_pins[i]);
            // SEFL::Logger::notice(this->getName(), value);
            if (value == LOW && this->buttons_states[i] == ButtonState::RESETED)
            {
                this->buttons_states[i] = ButtonState::PRESSED;
            }
            else if (value == HIGH && this->buttons_states[i] == ButtonState::PRESSED)
            {
                this->buttons_states[i] = ButtonState::RELEASED;
                this->current_order[current_input++]=i;
                this->buttons_states[i] = ButtonState::RESETED;
            }
        }
    }

    bool GammaPuzzle::checkButtons() {
        if (current_input<=0){
            puzzle_state=INITIAL;
            return false;
        }
        for (int i = 0; i < current_input; ++i) {
            if(current_order[i]!=correct_order[i]);
                puzzle_state=INCORRECT_ORDER;
                incorrect_animation_timestamp=millis();
                return false;
        }
        if (current_input==order_size){
            puzzle_state=CORRECT_ORDER;
            correct_animation_timestamp=millis();
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
        if(puzzle_state==INITIAL){

            for (int i = 0; i < order_size; ++i) {
                Pext.digitalWrite(neons_pins[current_order[i]],(i<current_input)?HIGH:LOW);
            }

        }else if (puzzle_state==INCORRECT_ORDER){
            if(millis()-incorrect_animation_timestamp<incorrect_animation_timeout){
                for (int i = 0; i < order_size; ++i) {
                    Pext.digitalWrite(neons_pins[current_order[i]],(i<current_input && (millis()-incorrect_animation_timestamp)/500%2)?HIGH:LOW);

                }
            }else{
                for (int i = 0; i < order_size; ++i) {
                    Pext.digitalWrite(neons_pins[current_order[i]],LOW);

                }
                current_input=0;
                puzzle_state=INITIAL;
            }
        }else if(puzzle_state==CORRECT_ORDER){
            if(millis()-incorrect_animation_timestamp<incorrect_animation_timeout){
                for (int i = 0; i < order_size; ++i) {
                    Pext.digitalWrite(neons_pins[current_order[i]],(i<current_input && (millis()-incorrect_animation_timestamp)/500+i%2)?HIGH:LOW);
                }
            }else{
                for (int i = 0; i < order_size; ++i) {
                    Pext.digitalWrite(neons_pins[current_order[i]],HIGH);
                }
                puzzle_state=INITIAL;
            }
        }
    }


} // SEFL
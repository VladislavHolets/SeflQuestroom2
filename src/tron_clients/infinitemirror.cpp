//
// Created by vladi on 14.03.2023.
//

#include "infinitemirror.h"

namespace SEFL {
    InfiniteMirror::InfiniteMirror(MQTTClient &mqtt, const char *name, uint8_t resetStatus, const char *placement,
                                   const char *inTopic, const char *outTopic, Language language)
            : Quest_Actuator_Client(mqtt, name, resetStatus, placement, inTopic, outTopic, language) {
        led_cathodes= nullptr;
        led_cathodes_size=-1;
        led_mode=0;
        animation_start_timestamp=0;
    }

    int16_t *InfiniteMirror::getLedCathodes() const {
        return led_cathodes;
    }

    void InfiniteMirror::setLedCathodes(int16_t *ledCathodes,int8_t size) {
        if(led_cathodes!= nullptr){
            delete[] led_cathodes;
        }
        led_cathodes=new int16_t[size];
        led_cathodes_size=size;
        for(int8_t i=0;i<led_cathodes_size;i++){
            led_cathodes[i]=ledCathodes[i];
        }
    }

    void InfiniteMirror::onOn() {
    if(isChangedStatus()){
        unsetChangedStatus();
        Quest_Actuator_Client::reportStatus();
    }
    while(!data.empty()){
        led_mode=data.front()[1];
        animation_start_timestamp=millis();
    }
    cycleMode(led_mode);
    }

    void InfiniteMirror::onOff() {
        if(isChangedStatus()){
            unsetChangedStatus();
            Quest_Actuator_Client::reportStatus();
        }
        for (int i = 0; i < led_cathodes_size; ++i) {
            Pext.digitalWrite(led_cathodes[i],HIGH);
        }
    }

    void InfiniteMirror::cycleMode(char ledMode) {
        switch (ledMode) {
            case 'w':{
                for (int i = 0; i < led_cathodes_size; ++i) {
                    Pext.digitalWrite(led_cathodes[i],LOW);
                }
            }break;
            case 'r':{
                Pext.digitalWrite(led_cathodes[0],LOW);
                Pext.digitalWrite(led_cathodes[1],HIGH);
                Pext.digitalWrite(led_cathodes[2],HIGH);
            }
                break;
            case 'g':{
                Pext.digitalWrite(led_cathodes[0],HIGH);
                Pext.digitalWrite(led_cathodes[1],LOW);
                Pext.digitalWrite(led_cathodes[2],HIGH);
            }
                break;
            case 'b':{
                Pext.digitalWrite(led_cathodes[0],HIGH);
                Pext.digitalWrite(led_cathodes[1],HIGH);
                Pext.digitalWrite(led_cathodes[2],LOW);
            }
                break;
            case 'a':{
                Pext.analogWrite(led_cathodes[0],(sin(millis()*TWO_PI/5000)));
                Pext.digitalWrite(led_cathodes[1],HIGH);
                Pext.digitalWrite(led_cathodes[2],HIGH);
            }
                break;
            default:{
                for (int i = 0; i < led_cathodes_size; ++i) {
                    Pext.digitalWrite(led_cathodes[i],HIGH);
                }
            }
                break;

        }
    }
} // SEFL
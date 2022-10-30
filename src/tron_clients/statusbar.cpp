//
// Created by vladi on 28.10.2022.
//

#include "statusbar.h"

namespace SEFL {
    void StatusBar::onOn() {
        if(isChangedStatus()){
            setChangedStatus(false);
            reportStatus();
        }
        if(!this->data.empty()){
            long number=this->data.front().toInt();
            while (number>0){
                long digit = number-(number/10)*10;
                number/=10;
                if(digit<=status_lamps_size){
                    this->status_lamps_states[digit-1]=true;

                    Logger::notice("status_bar_1 / ON",digit-1 );
                }
            }
            data.remove(0);

            refreshLeds();
        }
    }

    void StatusBar::onOff() {
        if(isChangedStatus()){
            setChangedStatus(false);
            reportStatus();
        }
        if(!this->data.empty()){
            long number=this->data.front().toInt();
            while (number>0){
                long digit = number-(number/10)*10;
                number/=10;
                if(digit<=status_lamps_size){
                    this->status_lamps_states[digit-1]=false;
                    Logger::notice("status_bar_1 / OFF",digit-1 );
                }
            }
            data.remove(0);
            refreshLeds();
        }

    }

    void StatusBar::setStatusLampsPins(const uint8_t *statusLampsPins, int8_t statusLampsSize) {
        if(status_lamps_pins!= nullptr){
            delete [] status_lamps_pins;
            delete [] status_lamps_states;
        }
        status_lamps_size=statusLampsSize;
        status_lamps_pins=new uint8_t [status_lamps_size];
        status_lamps_states=new bool [status_lamps_size];
        for (int i = 0; i < status_lamps_size; ++i) {
            status_lamps_pins[i]=statusLampsPins[i];
            status_lamps_states[i]=false;
        }
    }

    StatusBar::StatusBar(MQTTClient &mqtt, const char *name, uint8_t resetStatus, const char *placement,
                         const char *inTopic, const char *outTopic, Language language) : Quest_Actuator_Client(mqtt,
                                                                                                               name,
                                                                                                               resetStatus,
                                                                                                               placement,
                                                                                                               inTopic,
                                                                                                               outTopic,
                                                                                                               language) {
        status_lamps_states= nullptr;
        status_lamps_pins= nullptr;
        status_lamps_size=-1;
    }

    void StatusBar::refreshLeds() {
        for (int i = 0; i < status_lamps_size; ++i) {
            Pext.digitalWrite(status_lamps_pins[i],status_lamps_states[i]);

        }
    }
} // SEFL
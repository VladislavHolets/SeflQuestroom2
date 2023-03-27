//
// Created by vladi on 20.11.2022.
//

#include "diskholder.h"
#include "ArduinoJson.h"

namespace SEFL {


    DiskHolderArray::DiskHolderArray(MQTTClient &mqtt, const char *name, uint8_t resetStatus, const char *placement,
                                     const char *inTopic, const char *outTopic, Language language)
            : Quest_Basic_Client(mqtt, name, resetStatus, placement, inTopic, outTopic, language) {
        holders_ = nullptr;
        holders_size_ = -1;
        holdertype_ = DISPENSER;
        holders_servos_ = nullptr;
        led_state_ = false;
    }

    void DiskHolderArray::setHolders(HolderPins *holders, uint8_t size) {
        if (holders_ != nullptr) {
            delete holders_;
        }
        holders_size_ = size;
        holders_ = new HolderPins[holders_size_];
        holders_servos_ = new PextServo[holders_size_];
        for (int i = 0; i < holders_size_; ++i) {
            holders_[i].led_pin = holders[i].led_pin;
            holders_[i].sensor_pin = holders[i].sensor_pin;
            holders_[i].servo_pin = holders[i].servo_pin;
            holders_[i].attached_timestamp = holders[i].attached_timestamp;
            holders_[i].sensor_trashhold=holders[i].sensor_trashhold;
            holders_[i].dispence_angle = holders[i].dispence_angle;
            holders_[i].receive_angle=holders[i].receive_angle;
        }
    }

    bool DiskHolderArray::check_disk(uint8_t index) {
        if (index >= holders_size_)
            return false;
        pinMode(Mext.getCi(),INPUT);
        bool pin_value = Mext.analogRead(holders_[index].sensor_pin)>holders_[index].sensor_trashhold;

        Logger::notice(this->name_, pin_value);
       //Pext.digitalWrite(holders_[index].led_pin, pin_value);
        if (led_state_) {
            if(holdertype_ == DISPENSER) {
                Pext.digitalWrite(holders_[index].led_pin, pin_value);
            }else{
                Pext.digitalWrite(holders_[index].led_pin, !pin_value);
            }
        }
        else {
            Pext.digitalWrite(holders_[index].led_pin, HIGH);
        }
        if(pin_value!=holders_[index].previous_pin_value){
            holders_[index].previous_pin_value=pin_value;
            if(!pin_value && holdertype_ == DISPENSER){
                StaticJsonDocument<SEFL::DOC_SIZE> repDoc;
                repDoc["disk"]="dispensed";
                repDoc["id"]=index;

                String output;
                serializeJson(repDoc,output);
                data.push_back(output);
                Quest_Basic_Client::reportStatus();
                data.pop_back();
            }else if(pin_value && holdertype_==RECEIVER){
                StaticJsonDocument<SEFL::DOC_SIZE> repDoc;
                repDoc["disk"]="received";
                repDoc["id"]=index;

                String output;
                serializeJson(repDoc,output);
                data.push_back(output);
                Quest_Basic_Client::reportStatus();
                data.pop_back();
            }

        }
        return !pin_value;
    }

    void DiskHolderArray::dispence_disk(uint8_t index) {
        if (index >= holders_size_)
            return;
        holders_servos_[index].attach(holders_[index].servo_pin);
        holders_servos_[index].write(holders_[index].dispence_angle);
        holders_[index].attached_timestamp = millis();
    }

    void DiskHolderArray::set_led_state(bool led_state) {
        this->led_state_ = led_state;
    }

    void DiskHolderArray::detach_servos(uint32_t timeout) {
        for (int i = 0; i < holders_size_; ++i) {
            if (holders_[i].attached_timestamp && (millis() - holders_[i].attached_timestamp) > timeout) {
                holders_servos_[i].detach();
                holders_[i].attached_timestamp=0;
                return;
            }
        }
    }

    void DiskHolderArray::onActive() {
        if (isChangedStatus()) {
            unsetChangedStatus();
            Quest_Basic_Client::reportStatus();
            set_led_state(true);
        }
        if (!data.empty()) {
            Logger::notice(data[0]);
            StaticJsonDocument<SEFL::DOC_SIZE> doc;

            DeserializationError error = deserializeJson(doc, data.front());
            if (error) {
                Logger::notice(data[0]);
                Logger::notice(F("deserializeJson() failed: "));
                Logger::notice(error.f_str());
                data.remove(0);
                return;
            }
            JsonVariant next=doc["next"];
            if(next.isNull()){
            JsonArray holders = doc["holders"];
            for (auto &&holder: holders) {
                if (holder[0] < holders_size_) {
                    if (holder[1]) {
                        dispence_disk(holder[0]);
                    } else {
                        receive_disk(holder[0]);
                    }
                }
            }}
            else{
                process_next_disk();
            }
            data.remove(0);
        }

        int result = 0;
        String debug_string;
        for (int holder = 0; holder < holders_size_; ++holder) {
            result += check_disk(holder) ? 1 : 0;
            debug_string+= String( Mext.analogRead(holders_[holder].sensor_pin))+"  ";
        }
       Logger::notice(this->getName(),debug_string);

        if ((holdertype_ == DISPENSER && result == holders_size_) || (holdertype_ == RECEIVER && result == 0)) {
            Quest_Basic_Client::setStatus(FINISHED_STATUS);
        }
        detach_servos(2000);
    }

    void DiskHolderArray::onDefault() {
        if (isChangedStatus()) {
            unsetChangedStatus();
            Quest_Basic_Client::reportStatus();
            set_led_state(false);
            for (int holder = 0; holder < holders_size_; ++holder) {
                 check_disk(holder);
            }
        }
    }

    void DiskHolderArray::onFinished() {
        if (isChangedStatus()) {
            unsetChangedStatus();
            Quest_Basic_Client::reportStatus();
            set_led_state(true);

            for (int holder = 0; holder < holders_size_; ++holder) {
                check_disk(holder);
            }
        }

        detach_servos(2000);
    }

    void DiskHolderArray::onManualFinished() {
        onFinished();
    }

    void DiskHolderArray::receive_disk(uint8_t index) {
        if (index >= holders_size_)
            return;
        holders_servos_[index].attach(holders_[index].servo_pin);
        holders_servos_[index].write(holders_[index].receive_angle);
        holders_[index].attached_timestamp = millis();
    }
    void  DiskHolderArray::process_next_disk(){
        for (int i = 0; i < holders_size_; ++i) {
            if (!check_disk(i) && holdertype_ == DISPENSER  ) {
                Logger::notice(this->getName(),"dispensing next");
                dispence_disk(i);
                return;
            }else if(check_disk(i) && holdertype_==RECEIVER){
                Logger::notice(this->getName(),"receiving next");
                receive_disk(i);
                return;
            }
        }
    }

    DiskHolderArrayType DiskHolderArray::getHoldertype() const {
        return holdertype_;
    }

    void DiskHolderArray::setHoldertype(DiskHolderArrayType holdertype) {
        holdertype_ = holdertype;
    }
} // SEFL
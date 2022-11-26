//
// Created by vladi on 20.11.2022.
//

#include "diskholder.h"

namespace SEFL {
    void DiskHolderArray::onOn() {

    }

    void DiskHolderArray::onOff() {

    }

    DiskHolderArray::DiskHolderArray(MQTTClient &mqtt, const char *name, uint8_t resetStatus, const char *placement,
                                     const char *inTopic, const char *outTopic, Language language)
            : Quest_Basic_Client(mqtt, name, resetStatus, placement, inTopic, outTopic, language) {
        holders_ = nullptr;
        holders_size_ = -1;
        holdertype_ = DISPENCER;
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
        }
    }

    bool DiskHolderArray::check_disk(uint8_t index) {
        if (index >= holders_size_)
            return false;
        bool pin_value = Mext.digitalRead(holders_[index].sensor_pin);
        return !pin_value;
    }

    void DiskHolderArray::extract_disk(uint8_t index) {
        if (index >= holders_size_)
            return ;
    }

    void DiskHolderArray::extract_disk() {
        for (int i = 0; i < holders_size_; ++i) {
            if(check_disk(i)){
                extract_disk(i);
            }
        }
    }
} // SEFL
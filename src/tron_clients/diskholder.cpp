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
        holdertype_ = DISPENCER;
        holders_servos_ = nullptr;
        led_state_ = false;
        dispence_angle = 150;
        receive_angle = 0;
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
        }
    }

    bool DiskHolderArray::check_disk(uint8_t index) {
        if (index >= holders_size_)
            return false;
        pinMode(Mext.getCi(),INPUT);
        bool pin_value = Mext.digitalRead(holders_[index].sensor_pin);
        Logger::notice(this->name_, pin_value);

        Pext.digitalWrite(holders_[index].led_pin, pin_value);
//        if (led_state_) {
//            Pext.digitalWrite(holders_[index].led_pin, pin_value);
//        } else {
//            Pext.digitalWrite(holders_[index].led_pin, HIGH);
//        }
        return !pin_value;
    }

    void DiskHolderArray::dispence_disk(uint8_t index) {
        if (index >= holders_size_)
            return;
        holders_servos_[index].attach(holders_[index].servo_pin);
        holders_servos_[index].write(dispence_angle);
        holders_[index].attached_timestamp = millis();
    }

    void DiskHolderArray::dispence_disk() {
        for (int i = 0; i < holders_size_; ++i) {
            if (check_disk(i)) {
                dispence_disk(i);
                return;
            }
        }
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
            JsonArray holders = doc["holders"];
            for (auto &&holder: holders) {
                if (holder[0] < holders_size_) {
                    if (holder[1]) {
                        dispence_disk(holder[0]);
                    } else {
                        receive_disk(holder[0]);
                    }
                }
            }
            data.remove(0);
        }
        int result = 0;
        for (int holder = 0; holder < holders_size_; ++holder) {
            result += check_disk(holder) ? 1 : 0;
        }
        if ((holdertype_ == DISPENCER  && result == holders_size_) || (holdertype_ == RECEIVER && result == 0)) {
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
            set_led_state(false);

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
        holders_servos_[index].write(receive_angle);
        holders_[index].attached_timestamp = millis();
    }

    DiskHolderArrayType DiskHolderArray::getHoldertype() const {
        return holdertype_;
    }

    void DiskHolderArray::setHoldertype(DiskHolderArrayType holdertype) {
        holdertype_ = holdertype;
    }
} // SEFL
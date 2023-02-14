//
// Created by vladi on 16.12.2022.
//

#include "statusbarhc595.h"
#include "ArduinoJson.h"

namespace SEFL {
    void StatusBarHC595::onOn() {
        if (isChangedStatus()) {
            setChangedStatus(false);
            reportStatus();
        }
        if (!this->data.empty()) {
            StaticJsonDocument<250> doc;
            Logger::notice("status_bar", data.front());

            DeserializationError error = deserializeJson(doc, data.front());
            if (error) {
                Logger::notice(F("deserializeJson() failed: "));
                Logger::notice(error.f_str());

                data.remove(0);
                return;
            }
            JsonArray lights = doc["lights"];
            for (auto &&light: lights) {
                if (light[0].as<int>() < status_lamps_size) {
                    status_lamps_states[light[0].as<int>()] = true;
                    //Logger::notice("status_bar", light[0].as<int>());
                    //Logger::notice("status_bar", light[1].as<int>());
                    Logger::notice(this->name_, "lamps array:");
                    for (int i = 0; i < status_lamps_size; ++i) {
                        Logger::getPrinter()->print(status_lamps_states[i]);

                    }
                }
            }
            data.remove(0);
            refreshLeds();
        }


        refreshLeds();
    }

    void StatusBarHC595::onOff() {
        if (isChangedStatus()) {
            setChangedStatus(false);
            reportStatus();

        }

        for (int i = 0; i < status_lamps_size; i++) {
            status_lamps_states[i] = false;
        }

        refreshLeds();
//        if (!this->data.empty()) {
//            StaticJsonDocument<250> doc;
//            Logger::notice("status_bar",data.front());
//            DeserializationError error = deserializeJson(doc, data.front());
//            if (error) {
//                Logger::notice(F("deserializeJson() failed: "));
//                Logger::notice(error.f_str());
//
//                data.remove(0);
//                return;
//
//            }
//            JsonArray lights = doc["lights"];
//            for (auto &&light: lights) {
//                if (light[0].as<int>() < status_lamps_size) {
//                    status_lamps_states[light[0].as<int>()]=false;
//                }
//            }
//
//            data.remove(0);
    }


    StatusBarHC595::StatusBarHC595(MQTTClient
                                   &mqtt,
                                   const char *name, uint8_t
                                   resetStatus,
                                   const char *placement,
                                   const char *inTopic,
                                   const char *outTopic, Language
                                   language) :
            Quest_Actuator_Client(mqtt,
                                  name,
                                  resetStatus,
                                  placement,
                                  inTopic,
                                  outTopic,
                                  language
            ) {
        status_lamps_states = nullptr;
        status_lamps_pins = nullptr;
        status_lamps_size = -1;
    }

    void StatusBarHC595::refreshLeds() {
        String debug_string="Setting "+String(status_lamps_size)+" bits:\n";
        driver_->setDataByte(status_lamps_chip,0);
        for (int i = 0; i < status_lamps_size; ++i) {
            debug_string+=String(status_lamps_pins[i] + status_lamps_chip * 8)+" is " + status_lamps_states[i] +"\n";

            driver_->setDataItem(status_lamps_pins[i] + status_lamps_chip * 8, status_lamps_states[i]);

        }
        debug_string+="resulting in: \n";
        driver_->sendData();
        for (int i = 0; i < driver_->getDataLength(); ++i) {
            debug_string+=String(driver_->getData()[i], BIN)+"\n";
        }
        Logger::notice(this->name_, debug_string);
    }

    void StatusBarHC595::setStatusLampsPins(const uint8_t *statusLampsPins, int8_t statusLampsSize) {
        if (status_lamps_pins != nullptr) {
            delete[] status_lamps_pins;
            delete[] status_lamps_states;
        }
        status_lamps_size = statusLampsSize;
        status_lamps_pins = new uint8_t[status_lamps_size];
        status_lamps_states = new bool[status_lamps_size];
        for (int i = 0; i < status_lamps_size; ++i) {
            status_lamps_pins[i] = statusLampsPins[i];
            status_lamps_states[i] = false;
        }
    }

    void StatusBarHC595::setStatusLampsChip(uint8_t statusLampsChip) {
        status_lamps_chip = statusLampsChip;
    }

    void StatusBarHC595::setDriver(HC595_Driver &driver) {
        driver_ = &driver;
    }

} // SEFL
//
// Created by vladi on 05.05.2023.
//

#include "questbattlemanagerclient.h"
#include "ArduinoJson.h"
namespace SEFL {
    Quest_Battle_Manager_Client::Quest_Battle_Manager_Client(MQTTClient &mqtt, const char *name, uint8_t resetStatus,
                                                             const char *placement, const char *inTopic,
                                                             const char *outTopic, Language language)
            : Quest_Sensor_Client(mqtt, name, resetStatus, placement, inTopic, outTopic, language) {
        mode=MONO;
    }

    void Quest_Battle_Manager_Client::onSet() {
        if(this->isChangedStatus()){
            this->unsetChangedStatus();
            Quest_Sensor_Client::reportStatus();
        }
        while(!this->getData().empty()){
            StaticJsonDocument<SEFL::DOC_SIZE> doc;
            DeserializationError error = deserializeJson(doc,data.front().c_str(),data.front().length());
            data.remove(0);
            if (error) {
                Logger::notice(F("deserializeJson() failed: "));
                Logger::notice(error.f_str());

                return;

            }
            if(doc["TriggerId"]!=0){
                String trigger_id=doc["TriggerId"];
                if(trigger_id.equals("web_setup")){
                    mode=static_cast<QuestModes>(doc["GameMode"].as<int>());
                    if(mode==ERROR){
                        mode=MONO;
                    }
                }else{
                    doc["GameMode"]=mode;
                    String reply;
                    serializeJson(doc,reply);
                    data.push_back(reply);
                    Quest_Sensor_Client::reportStatus();
                    data.remove(data.size()-1);
                }
            }
        }
    }

    void Quest_Battle_Manager_Client::onReset() {
        if(this->isChangedStatus()){
            this->unsetChangedStatus();
            Quest_Sensor_Client::reportStatus();
        }
        this->data.clear();
    }
} // SEFL
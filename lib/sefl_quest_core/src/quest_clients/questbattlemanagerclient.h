//
// Created by vladi on 05.05.2023.
//

#ifndef SEFLQUESTROOM2_QUESTBATTLEMANAGERCLIENT_H
#define SEFLQUESTROOM2_QUESTBATTLEMANAGERCLIENT_H
#include "sefl_quest_core.h"
namespace SEFL {

    class Quest_Battle_Manager_Client:public Quest_Sensor_Client {
        enum QuestModes{
            ERROR=0,MONO=1,BATTLE=2,HARDCORE=3,BATTLE_TIME_DEDUCTION=4,HARDCORE_TIME_DEDUCTION=5
        }mode;
    public:
        Quest_Battle_Manager_Client(MQTTClient &mqtt, const char *name, uint8_t resetStatus, const char *placement=SEFL::DEFAULT_PLACEMENT,
                                    const char *inTopic=SEFL::DEFAUT_IN_TOPIC, const char *outTopic=SEFL::DEFAUT_OUT_TOPIC, Language language=SEFL::DEFAULT_LANGUAGE);

        void onSet() override;

        void onReset() override;

    };

} // SEFL

#endif //SEFLQUESTROOM2_QUESTBATTLEMANAGERCLIENT_H

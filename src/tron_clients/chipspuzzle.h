//
// Created by vladi on 04.12.2022.
//

#ifndef SEFLQUESTROOM2_CHIPSPUZZLE_H
#define SEFLQUESTROOM2_CHIPSPUZZLE_H
#include "sefl_quest_core.h"
#include "tronlegacyadapter.h"

namespace SEFL {

    class ChipsPuzzle: public Quest_Basic_Client {
        TronLegacyAdapter *adapter_;

    public:
        ChipsPuzzle(MQTTClient &mqtt, const char *name, uint8_t resetStatus=1, const char *placement=DEFAULT_PLACEMENT, const char *inTopic=DEFAUT_IN_TOPIC,
                    const char *outTopic=DEFAUT_OUT_TOPIC, Language language=ENG);

        void onActive() override;

        void onDefault() override;

        void onFinished() override;

        void onManualFinished() override;

        void setAdapter(TronLegacyAdapter &adapter);
    };

} // SEFL

#endif //SEFLQUESTROOM2_CHIPSPUZZLE_H

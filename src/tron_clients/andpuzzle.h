//
// Created by vladi on 25.11.2022.
//

#ifndef SEFLQUESTROOM2_ANDPUZZLE_H
#define SEFLQUESTROOM2_ANDPUZZLE_H
#include "sefl_quest_core.h"
#include "tronlegacyadapter.h"

namespace SEFL {

    class ANDPuzzle: public Quest_Basic_Client{
        TronLegacyAdapter *adapter_;
        uint32_t led_on_timestamp;
        uint32_t led_on_timeout;
    public:
        ANDPuzzle(MQTTClient &mqtt, const char *name, uint8_t resetStatus=1, const char *placement=DEFAULT_PLACEMENT, const char *inTopic=DEFAUT_IN_TOPIC,
                  const char *outTopic=DEFAUT_OUT_TOPIC, Language language=ENG);

        void onActive() override;

        void onDefault() override;

        void onFinished() override;

        void onManualFinished() override;

        void setAdapter(TronLegacyAdapter &adapter);

        uint32_t getLedOnTimeout() const;

        void setLedOnTimeout(uint32_t ledOnTimeout);
    };

} // SEFL

#endif //SEFLQUESTROOM2_ANDPUZZLE_H

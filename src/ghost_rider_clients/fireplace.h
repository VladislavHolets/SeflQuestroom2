/*
 * fireplace.h
 *
 *  Created on: Jan 31, 2022
 *      Author: piatk
 */

#ifndef FIREPLACE_H_
#define FIREPLACE_H_

#include "sefl_quest_core.h"

#include <sys/_stdint.h>

class DFRobotDFPlayerMini;

namespace SEFL
{

	class Fireplace : public Quest_Basic_Client
	{
		uint8_t fireplaceAcLightRelay, fireplaceSmokeRelay;
		uint8_t fireplaceCoolerOut, fireplaceLedOut;
		uint8_t fireplaceButtont;
		uint8_t liquidLevelSensor, LiquidLevelLed;
		DFRobotDFPlayerMini *player;

	public:
		Fireplace(MQTTClient &mqtt, const char *name = DEFAULT_DEVICE_NAME,
				  uint8_t reset_status = 1, const char *placement = DEFAULT_PLACEMENT,
				  const char *in_topic = DEFAUT_IN_TOPIC, const char *out_topic = DEFAUT_OUT_TOPIC, SEFL::Language language = UKR);
		virtual ~Fireplace();
		virtual void onActive() override;
		virtual void onFinished() override;
		virtual void onManualFinished() override;
		virtual void onDefault() override;
		uint8_t getFireplaceAcLightRelay();
		void setFireplaceAcLightRelay(uint8_t fireplaceAcLightRelay);
		uint8_t getFireplaceButtont();
		void setFireplaceButtont(uint8_t fireplaceButtont);
		uint8_t getFireplaceCoolerOut();
		void setFireplaceCoolerOut(uint8_t fireplaceCoolerOut);
		uint8_t getFireplaceLedOut();
		void setFireplaceLedOut(uint8_t fireplaceLedOut);
		uint8_t getFireplaceSmokeRelay();
		void setFireplaceSmokeRelay(uint8_t fireplaceSmokeRelay);
		DFRobotDFPlayerMini *getPlayer();
		void setPlayer(DFRobotDFPlayerMini *player);
		uint8_t getLiquidLevelLed();
		void setLiquidLevelLed(uint8_t liquidLevelLed);
		uint8_t getLiquidLevelSensor();
		void setLiquidLevelSensor(uint8_t liquidLevelSensor);
	};

} /* namespace SEFL */

#endif /* FIREPLACE_H_ */

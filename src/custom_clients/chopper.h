/*
 * chopper.h
 *
 *  Created on: Jan 31, 2022
 *      Author: piatk
 */

#ifndef CHOPPER_H_
#define CHOPPER_H_

#include <constants.h>
#include <quest_clients/questbasicclient.h>
#include <sys/_stdint.h>

class DFRobotDFPlayerMini;

namespace SEFL
{

	class Chopper : public Quest_Basic_Client
	{
		uint8_t chopperLight;
		uint8_t chopperAddressLed;
		uint8_t chopperCharger;
		uint8_t chopperStarter;
		DFRobotDFPlayerMini *player;

	public:
		Chopper(MQTTClient &mqtt, const char *name = DEFAULT_DEVICE_NAME,
				uint8_t reset_status = 1, const char *placement = DEFAULT_PLACEMENT,
				const char *in_topic = DEFAUT_IN_TOPIC, const char *out_topic = DEFAUT_OUT_TOPIC, SEFL::Language language = UKR);
		virtual ~Chopper();
		virtual void onActive() override;
		virtual void onFinished() override;
		virtual void onManualFinished() override;
		virtual void onDefault() override;
		uint8_t getChopperAddressLed() const;
		void setChopperAddressLed(uint8_t chopperAddressLed);

		uint8_t getChopperCharger() const;
		void setChopperCharger(uint8_t chopperCharger);

		uint8_t getchopperStarter() const;
		void setchopperStarter(uint8_t chopperStarter);

		DFRobotDFPlayerMini *getPlayer() const;
		void setPlayer(DFRobotDFPlayerMini *player);
		uint8_t getChopperStarter() const;
		void setChopperStarter(uint8_t chopperStarter);

		uint8_t getChopperLight() const
		{
			return chopperLight;
		}

		void setChopperLight(uint8_t chopperLight)
		{
			this->chopperLight = chopperLight;
		}
	};

} /* namespace SEFL */

#endif /* CHOPPER_H_ */

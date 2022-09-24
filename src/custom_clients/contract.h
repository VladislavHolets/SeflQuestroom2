/*
 * contract.h
 *
 *  Created on: Jan 25, 2022
 *      Author: piatk
 */

#ifndef CONTRACT_H_
#define CONTRACT_H_

#include <quest_clients/questbasicclient.h>

namespace SEFL
{

	class Contract : public Quest_Basic_Client
	{
		uint8_t hell_light, hell_magnet, swith_contract, smoke_button;
		bool invertet;

	public:
		Contract(MQTTClient &mqtt, const char *name = DEFAULT_DEVICE_NAME,
				 uint8_t reset_status = 1, const char *placement = DEFAULT_PLACEMENT,
				 const char *in_topic = DEFAUT_IN_TOPIC, const char *out_topic = DEFAUT_OUT_TOPIC, SEFL::Language language = UKR);
		virtual ~Contract();
		virtual void onActive() override;
		virtual void onFinished() override;
		virtual void onManualFinished() override;
		virtual void onDefault() override;
		uint8_t getHellLight();
		void setHellLight(uint8_t hellLight);
		uint8_t getHellMagnet();
		void setHellMagnet(uint8_t hellMagnet);
		bool isInvertet();
		void setInvertet(bool invertet);
		uint8_t getSmokeButton();
		void setSmokeButton(uint8_t smokeButton);
		uint8_t getSwithContract();
		void setSwithContract(uint8_t swithContract);
	};

} /* namespace SEFL */

#endif /* CONTRACT_H_ */

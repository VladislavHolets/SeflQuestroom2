/*
 * chains.h
 *
 *  Created on: Jan 31, 2022
 *      Author: piatk
 */

#ifndef CHAINS_H_
#define CHAINS_H_
#include <constants.h>
#include <quest_clients/questbasicclient.h>
#include <sys/_stdint.h>

namespace SEFL
{

	class Chains : public Quest_Basic_Client
	{

		uint8_t chainsLeftButton, chainsRightButton;
		uint8_t chainsLeftMagnet, chainsRightMagnet;

	public:
		Chains(MQTTClient &mqtt, const char *name = DEFAULT_DEVICE_NAME,
			   uint8_t reset_status = 1, const char *placement = DEFAULT_PLACEMENT,
			   const char *in_topic = DEFAUT_IN_TOPIC, const char *out_topic = DEFAUT_OUT_TOPIC, SEFL::Language language = UKR);
		virtual ~Chains();
		virtual void onActive() override;
		virtual void onFinished() override;
		virtual void onManualFinished() override;
		virtual void onDefault() override;
		uint8_t getChainsLeftButton();
		void setChainsLeftButton(uint8_t chainsLeftButton);
		uint8_t getChainsLeftMagnet();
		void setChainsLeftMagnet(uint8_t chainsLeftMagnet);
		uint8_t getChainsRightButton();
		void setChainsRightButton(uint8_t chainsRightButton);
		uint8_t getChainsRightMagnet();
		void setChainsRightMagnet(uint8_t chainsRightMagnet);
	};

} /* namespace SEFL */

#endif /* CHAINS_H_ */

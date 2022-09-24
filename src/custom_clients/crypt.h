/*
 * crypt.h
 *
 *  Created on: Jan 21, 2022
 *      Author: piatk
 */

#ifndef CRYPT_H_
#define CRYPT_H_

#include <quest_clients/questbasicclient.h>

namespace SEFL
{

	class Crypt : public Quest_Basic_Client
	{
		uint8_t leftTorch, rightTorch;
		uint8_t leftRelay, rightRelay;

	public:
		Crypt(MQTTClient &mqtt, const char *name = DEFAULT_DEVICE_NAME,
			  uint8_t reset_status = 1, const char *placement = DEFAULT_PLACEMENT,
			  const char *in_topic = DEFAUT_IN_TOPIC, const char *out_topic = DEFAUT_OUT_TOPIC, SEFL::Language language = UKR);
		virtual ~Crypt();
		virtual void onActive() override;
		virtual void onFinished() override;
		virtual void onManualFinished() override;
		virtual void onDefault() override;
		uint8_t getLeftRelay() const;
		void setLeftRelay(uint8_t leftRelay);
		uint8_t getLeftTorch() const;
		void setLeftTorch(uint8_t leftTorch);
		uint8_t getRightRelay() const;
		void setRightRelay(uint8_t rightRelay);
		uint8_t getRightTorch() const;
		void setRightTorch(uint8_t rightTorch);
	};

} /* namespace SEFL */

#endif /* CRYPT_H_ */

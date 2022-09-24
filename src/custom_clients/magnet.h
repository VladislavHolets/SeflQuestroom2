/*
 * magnet.h
 *
 *  Created on: Jan 21, 2022
 *      Author: piatk
 */

#ifndef MAGNET_H_
#define MAGNET_H_

#include <quest_clients/questactuatorclient.h>

namespace SEFL
{

	class Magnet : public Quest_Actuator_Client
	{
		uint8_t pin_number;
		bool inverted;

	public:
		Magnet(MQTTClient &mqtt, const char *name = DEFAULT_DEVICE_NAME,
			   uint8_t reset_status = 1, const char *placement = DEFAULT_PLACEMENT,
			   const char *in_topic = DEFAUT_IN_TOPIC, const char *out_topic = DEFAUT_OUT_TOPIC, SEFL::Language language = UKR);
		virtual ~Magnet();
		virtual void onOn() override;
		virtual void onOff() override;
		bool isInverted() const;
		void setInverted(bool inverted);
		uint8_t getPinNumber() const;
		void setPinNumber(uint8_t pinNumber);
	};

} /* namespace SEFL */

#endif /* MAGNET_H_ */

/*
 * mqtttwowayinterractor.h
 *
 *  Created on: May 16, 2021
 *      Author: vladi
 */

#ifndef MQTTTWOWAYINTERRACTOR_H_
#define MQTTTWOWAYINTERRACTOR_H_

#include <mqtt_wrappers/mqttsubscriberobjectbound.h>
#include <sys/_stdint.h>
#include <WString.h>

namespace SEFL
{
	class MQTT_Manager;
} /* namespace SEFL */

namespace SEFL
{

	class MQTT_Two_Way_Interactor
	{
	private:
		SEFL::MQTT *mqtt_;
		String subfeed_;
		String pubfeed_;
		SEFL::MQTT_Subscriber_Object_Bound<MQTT_Two_Way_Interactor> sub_;

	protected:
	public:
		virtual void inputClb(const char *data, uint16_t len) = 0;
		MQTT_Two_Way_Interactor(SEFL::MQTT &mqtt, String subfeed = "",
								String pubfeed = "");
		virtual ~MQTT_Two_Way_Interactor();
		SEFL::MQTT *getMqtt();
		String getPubfeed();
		String getSubfeed();
		SEFL::MQTT_Subscriber_Object_Bound<MQTT_Two_Way_Interactor> *getSub();
	};

} /* namespace SEFL */

#endif /* MQTTTWOWAYINTERRACTOR_H_ */

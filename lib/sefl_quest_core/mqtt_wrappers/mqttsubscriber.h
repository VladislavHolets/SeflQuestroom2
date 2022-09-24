/*
 * mqttsubscriber.h
 *
 *  Created on: May 4, 2021
 *      Author: vladi
 */

#ifndef MQTTSUBSCRIBER_H_
#define MQTTSUBSCRIBER_H_

#include <mqtt_wrappers/mqttconfig.h>
#include <sys/_stdint.h>

class Adafruit_MQTT;

namespace SEFL
{
	class MQTT;
} /* namespace SEFL */

namespace SEFL
{

	typedef void (*SubscriberCallbackBufferType)(char *str, uint16_t len);
	class MQTT_Subscriber
	{
	public:
		// MQTT_Subscriber(MQTTClient &mqttserver, const char *feedname,
		// 				uint8_t q = 0);

		MQTT_Subscriber(const char *feedname,
						uint8_t q = 0);
		virtual void removeCallback(void);
		virtual void callbackBuffer(char *data, uint16_t len);
		virtual bool getCallbackBuffer();

		void setTopic(const char *topic);
		char topic[45];
		uint8_t qos;
		uint8_t lastread[SUBSCRIPTIONDATALEN];
		// Number valid bytes in lastread. Limited to SUBSCRIPTIONDATALEN-1 to
		// ensure nul terminating lastread.
		uint16_t datalen;
		SubscriberCallbackBufferType callback_buffer;
		// MQTT *mqtt;
		virtual ~MQTT_Subscriber();
	};

} /* namespace SEFL */

#endif /* MQTTSUBSCRIBER_H_ */

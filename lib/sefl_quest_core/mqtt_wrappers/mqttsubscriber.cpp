/*
 * mqttsubscriber.cpp
 *
 *  Created on: May 4, 2021
 *      Author: vladi
 */

#include <mqtt_wrappers/mqttsubscriber.h>
#include <cstring>

namespace SEFL
{

	// MQTT_Subscriber::MQTT_Subscriber(MQTTClient &mqttserver, const char *feedname,
	// 								 uint8_t q)
	MQTT_Subscriber::MQTT_Subscriber(const char *feedname,
									 uint8_t q)
	{
		// mqtt = &mqttserver;
		topic[0] = '\0';
		strcpy(topic, feedname);
		qos = q;
		datalen = 0;
		callback_buffer = 0;
	}

	void MQTT_Subscriber::removeCallback(void)
	{
		callback_buffer = 0;
	}

	void MQTT_Subscriber::callbackBuffer(char *data, uint16_t len)
	{
		callback_buffer(data, len);
	}

	bool MQTT_Subscriber::getCallbackBuffer()
	{
		return (callback_buffer != NULL);
	}

	void MQTT_Subscriber::setTopic(const char *topic)
	{
		this->topic[0] = '\0';
		strcpy(this->topic, topic);
	}
	MQTT_Subscriber::~MQTT_Subscriber()
	{
		// TODO Auto-generated destructor stub
	}

} /* namespace SEFL */

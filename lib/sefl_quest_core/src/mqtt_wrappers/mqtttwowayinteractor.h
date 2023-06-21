/*
 * mqtttwowayinterractor.h
 *
 *  Created on: May 16, 2021
 *      Author: vladi
 */

#ifndef MQTTTWOWAYINTERRACTOR_H_
#define MQTTTWOWAYINTERRACTOR_H_
#include <MQTT.h>
#include <sys/_stdint.h>
#include <WString.h>
#include "../constants.h"
#include "Array.h"

namespace SEFL
{

	class MQTT_Two_Way_Interactor
	{
	private:
		MQTTClient *mqtt_;
		String subfeed_;
		String pubfeed_;
		// SEFL::MQTT_Subscriber_Object_Bound<MQTT_Two_Way_Interactor> sub_;
        struct DelayedMessage{
            String payload;
            uint32_t publish_time;
            int qos;
        };

        Array<DelayedMessage, MAX_DELAYED_MESSAGE_QUEUE_AMOUNT> delayed_messages;
	protected:
	public:
		MQTTClient *getMqtt();

		virtual void inputClb(const char *data, uint16_t len) = 0;
		// MQTT_Two_Way_Interactor(SEFL::MQTTClient &mqtt, String subfeed = "",
		// 						String pubfeed = "");

		MQTT_Two_Way_Interactor(MQTTClient &mqtt, String subfeed = "",
								String pubfeed = "");
		virtual ~MQTT_Two_Way_Interactor();

		String getPubfeed();
		String getSubfeed();
		// SEFL::MQTT_Subscriber_Object_Bound<MQTT_Two_Way_Interactor> *getSub();
		bool publish(String payload, int qos = SEFL::QOS_DEFAULT);
		bool publish(const char *payload, int qos = SEFL::QOS_DEFAULT);
        bool publishDelayed(String payload,uint32_t delay, int qos = SEFL::QOS_DEFAULT);
        bool publishDelayed(const char *payload,uint32_t delay,int qos = SEFL::QOS_DEFAULT);
        void processDelayedPublications();
		// bool publish(const char *payload, unsigned int length, int qos = SEFL::QOS_DEFAULT);
	};

} /* namespace SEFL */

#endif /* MQTTTWOWAYINTERRACTOR_H_ */

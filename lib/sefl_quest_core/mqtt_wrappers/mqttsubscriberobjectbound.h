/*
 * mqttsubscriberobjectbound.h
 *
 *  Created on: May 4, 2021
 *      Author: vladi
 */

#ifndef MQTTSUBSCRIBEROBJECTBOUND_H_
#define MQTTSUBSCRIBEROBJECTBOUND_H_

#include <mqtt_wrappers/mqttsubscriber.h>
#include <sys/_stdint.h>

namespace SEFL
{
	template <typename ObjT>
	class MQTT_Subscriber_Object_Bound : public MQTT_Subscriber
	{
	public:
		typedef void (ObjT::*CallbackBufferT)(const char *data, const uint16_t len);
		// MQTT_Subscriber_Object_Bound(MQTTClient &mqttserver, const char *feed,
		// 							 CallbackBufferT clb, ObjT *obj, uint8_t q = 0);
		MQTT_Subscriber_Object_Bound(const char *feed,
									 CallbackBufferT clb, ObjT *obj, uint8_t q = 0);

		void removeCallback(void) override;

		void callbackBuffer(char *data, uint16_t len) override;
		virtual bool getCallbackBuffer() override;
		CallbackBufferT my_callback_buffer;
		ObjT *obj_;

		virtual ~MQTT_Subscriber_Object_Bound() override;
	};
} /* namespace SEFL */

#endif /* MQTTSUBSCRIBEROBJECTBOUND_H_ */

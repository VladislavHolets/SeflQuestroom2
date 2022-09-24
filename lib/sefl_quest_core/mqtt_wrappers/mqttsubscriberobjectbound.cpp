/*
 * mqttsubscriberobjectbound.cpp
 *
 *  Created on: May 4, 2021
 *      Author: vladi
 */

#include <mqtt_wrappers/mqttsubscriberobjectbound.h>
#include <mqtt_wrappers/mqtttwowayinteractor.h>

namespace SEFL
{

	template <typename ObjT>
	// MQTT_Subscriber_Object_Bound<ObjT>::MQTT_Subscriber_Object_Bound(
	// 	MQTTClient &mqttserver, const char *feed, CallbackBufferT clb,
	// 	ObjT *obj, uint8_t q) : MQTT_Subscriber(mqttserver, feed, q), my_callback_buffer(clb), obj_(obj)
	MQTT_Subscriber_Object_Bound<ObjT>::MQTT_Subscriber_Object_Bound(
		const char *feed, CallbackBufferT clb,
		ObjT *obj, uint8_t q) : MQTT_Subscriber(feed, q), my_callback_buffer(clb), obj_(obj)
	{
	}

	template <typename ObjT>
	void MQTT_Subscriber_Object_Bound<ObjT>::removeCallback(void)
	{
		my_callback_buffer = 0;
		obj_ = 0;
	}

	template <typename ObjT>
	void MQTT_Subscriber_Object_Bound<ObjT>::callbackBuffer(char *data,
															uint16_t len)
	{
		(obj_->*my_callback_buffer)(data, len);
	}

	template <typename ObjT>
	bool MQTT_Subscriber_Object_Bound<ObjT>::getCallbackBuffer()
	{
		return (my_callback_buffer != nullptr);
	}

	template <typename ObjT>
	MQTT_Subscriber_Object_Bound<ObjT>::~MQTT_Subscriber_Object_Bound()
	{
	}

	template class MQTT_Subscriber_Object_Bound<MQTT_Two_Way_Interactor>;
} /* namespace SEFL */

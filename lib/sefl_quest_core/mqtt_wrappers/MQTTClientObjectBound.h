#ifndef MQTTCLIENTOBJECTBOUND_H
#define MQTTCLIENTOBJECTBOUND_H
#include <MQTT.h>

#pragma once
namespace SEFL
{
    template <typename ObjT>
    class MQTTClientObjectBound : public MQTTClient
    {
    public:
        typedef void (ObjT::*CallbackBufferT)(String &topic, String &payload);
        ObjT *obj_;
        CallbackBufferT clb_;
        explicit MQTTClientObjectBound(int bufSize);
        ~MQTTClientObjectBound();
        void mqttclb_(String &topic, String &payload);
        void onMessage(ObjT *obj, CallbackBufferT clb);

    private:
    };
}
#endif
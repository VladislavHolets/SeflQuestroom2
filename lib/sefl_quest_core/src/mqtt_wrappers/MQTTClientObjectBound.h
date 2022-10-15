#ifndef MQTTCLIENTOBJECTBOUND_H
#define MQTTCLIENTOBJECTBOUND_H
#include <AsyncMqtt_Generic.h>

#pragma once
namespace SEFL
{
    template <typename ObjT>
    class MQTTClientObjectBound : public AsyncMqttClient
    {
    public:
        typedef void (ObjT::*CallbackBufferT)(String &topic, String &payload);
        ObjT *obj_;
        CallbackBufferT clb_;
        MQTTClientObjectBound(int bufSize);
        ~MQTTClientObjectBound();
        void mqttclb_(String &topic, String &payload);
        void onMessage(ObjT *obj, CallbackBufferT clb);

    private:
    };
    void clbwrap(String &topic, String &payload);
    extern AsyncMqttClient *clbwrapobj;

}
#endif
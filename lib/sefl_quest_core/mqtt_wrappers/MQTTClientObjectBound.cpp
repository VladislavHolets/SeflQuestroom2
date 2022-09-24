#include <mqtt_wrappers/MQTTClientObjectBound.h>
namespace SEFL
{
    template <typename ObjT>
    MQTTClientObjectBound<ObjT>::MQTTClientObjectBound(int bufSize) : MQTTClient(bufSize)
    {
    }

    template <typename ObjT>
    void MQTTClientObjectBound<ObjT>::mqttclb_(String &topic, String &payload)
    {
        this->obj_->*clb_(topic, payload);
    }

    template <typename ObjT>
    void MQTTClientObjectBound<ObjT>::onMessage(ObjT *obj, CallbackBufferT clb)
    {
        this->obj_ = obj;
        this->clb_ = clb;
        this->onMessage(mqttclb_);
    }

    template <typename ObjT>
    MQTTClientObjectBound<ObjT>::~MQTTClientObjectBound()
    {
    }
}
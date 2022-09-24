#include <mqtt_wrappers/MQTTClientObjectBound.h>
#include <quest_clients/questboardmanager.h>
namespace SEFL
{
    template <typename ObjT>
    MQTTClientObjectBound<ObjT>::MQTTClientObjectBound(int bufSize) : MQTTClient(bufSize)
    {
    }

    template <typename ObjT>
    void MQTTClientObjectBound<ObjT>::mqttclb_(String &topic, String &payload)
    {
        (obj_->*clb_)(topic, payload);
    }

    template <typename ObjT>
    void MQTTClientObjectBound<ObjT>::onMessage(ObjT *obj, CallbackBufferT clb)
    {
        this->obj_ = obj;
        this->clb_ = clb;
        this->MQTTClient::onMessage(clbwrap);
    }

    template <typename ObjT>
    MQTTClientObjectBound<ObjT>::~MQTTClientObjectBound()
    {
    }
    template class MQTTClientObjectBound<Quest_Board_Manager>;
    void clbwrap(String &topic, String &payload)
    {
        reinterpret_cast<MQTTClientObjectBound<Quest_Board_Manager> *>(clbwrapobj)->mqttclb_(topic, payload);
    }
}
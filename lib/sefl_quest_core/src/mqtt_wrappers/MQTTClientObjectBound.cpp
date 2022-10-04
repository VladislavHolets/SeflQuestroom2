#include "MQTTClientObjectBound.h"
#include "../quest_clients/questboardmanager.h"
#include "../utils/logger.h"
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
    template <typename ObjT>
    void clbwrap(String &topic, String &payload)
    {
        Logger::warning(String("clbwrap"));
        reinterpret_cast<MQTTClientObjectBound<ObjT>*>(clbwrapobj)->mqttclb_(topic, payload);
    }
}
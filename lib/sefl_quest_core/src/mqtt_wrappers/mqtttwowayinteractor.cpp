/*
 * mqtttwowayinterractor.cpp
 *
 *  Created on: May 16, 2021
 *      Author: vladi
 */

#include "../utils/logger.h"
#include "mqtttwowayinteractor.h"
namespace SEFL
{

	MQTTClient *MQTT_Two_Way_Interactor::getMqtt()
	{
		return (mqtt_);
	}

	// MQTT_Two_Way_Interactor::MQTT_Two_Way_Interactor(SEFL::MQTTClient &mqtt,
	// 												 String subfeed, String pubfeed) : mqtt_(&mqtt), subfeed_(subfeed), pubfeed_(pubfeed), sub_(mqtt,
	// 																																			subfeed_.c_str(), &SEFL::MQTT_Two_Way_Interactor::inputClb,
	// 																																			this, 1)
	MQTT_Two_Way_Interactor::MQTT_Two_Way_Interactor(MQTTClient &mqtt,
													 String subfeed, String pubfeed) : mqtt_(&mqtt), subfeed_(subfeed), pubfeed_(pubfeed)
	{
		// this->mqtt_->subscribe(&sub_);

		Logger::notice("two way interactor", F("constructor_done"));
		Logger::notice("two way interactor", String("pubfeed:") + pubfeed);
		//	Logger::notice("two way interactor",this->getSubfeed());
	}

	// SEFL::MQTT_Subscriber_Object_Bound<MQTT_Two_Way_Interactor> *MQTT_Two_Way_Interactor::getSub()
	// {
	// 	return (&sub_);
	// }

	String MQTT_Two_Way_Interactor::getPubfeed()
	{
		return (pubfeed_);
	}

	String MQTT_Two_Way_Interactor::getSubfeed()
	{
		return (subfeed_);
	}

	MQTT_Two_Way_Interactor::~MQTT_Two_Way_Interactor()
	{
		// TODO Auto-generated destructor stub
	}
	bool MQTT_Two_Way_Interactor::publish(String payload, int qos)
	{
		this->getMqtt()->publish(this->getPubfeed().c_str(), payload.c_str(), payload.length(), 0, qos);
		return true;
	}
	bool MQTT_Two_Way_Interactor::publish(const char *payload, int qos)
	{
		this->getMqtt()->publish(this->getPubfeed().c_str(), payload, strlen(payload), 0, qos);
		return true;
	}

    bool MQTT_Two_Way_Interactor::publishDelayed(const char *payload, uint32_t delay, int qos) {
        if(delayed_messages.full()){
            return false;
        }
        DelayedMessage message;
        message.payload=String(payload);
        message.qos=qos;
        message.publish_time=millis()+delay;
        delayed_messages.push_back(message);
        return true;
    }

    bool MQTT_Two_Way_Interactor::publishDelayed(String payload, uint32_t delay, int qos) {
        if(delayed_messages.full()){
            return false;
        }
        DelayedMessage message;
        message.payload=payload;
        message.qos=qos;
        message.publish_time=millis()+delay;
        delayed_messages.push_back(message);
        return true;
    }

    void MQTT_Two_Way_Interactor::processDelayedPublications() {
        for(int i=delayed_messages.size()-1;i>=0;i--){
            if(millis()>=delayed_messages[i].publish_time){
                publish(delayed_messages[i].payload, delayed_messages[i].qos);
                delayed_messages.remove(i);
            }
        }
    }
    // bool MQTT_Two_Way_Interactor::publish(const char *payload, unsigned int length, int qos)
	// {
	// 	this->getMqtt()->publish(this->getPubfeed().c_str(), payload, length, 0, qos);
	// }
} /* namespace SEFL */

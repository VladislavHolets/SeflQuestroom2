/*
 * mqtttwowayinterractor.cpp
 *
 *  Created on: May 16, 2021
 *      Author: vladi
 */

#include <utils/logger.h>
#include <mqtt_wrappers/mqtt.h>
#include <mqtt_wrappers/mqtttwowayinteractor.h>

namespace SEFL
{

	SEFL::MQTT *MQTT_Two_Way_Interactor::getMqtt()
	{
		return (mqtt_);
	}

	String MQTT_Two_Way_Interactor::getPubfeed()
	{
		return (pubfeed_);
	}

	// MQTT_Two_Way_Interactor::MQTT_Two_Way_Interactor(SEFL::MQTT &mqtt,
	// 												 String subfeed, String pubfeed) : mqtt_(&mqtt), subfeed_(subfeed), pubfeed_(pubfeed), sub_(mqtt,
	// 																																			subfeed_.c_str(), &SEFL::MQTT_Two_Way_Interactor::inputClb,
	// 																																			this, 1)
	MQTT_Two_Way_Interactor::MQTT_Two_Way_Interactor(SEFL::MQTT &mqtt,
													 String subfeed, String pubfeed) : mqtt_(&mqtt), subfeed_(subfeed), pubfeed_(pubfeed), sub_(subfeed_.c_str(), &SEFL::MQTT_Two_Way_Interactor::inputClb,
																																				this, 1)
	{
		this->mqtt_->subscribe(&sub_);

		Logger::notice("two way interactor", F("constructor_done"));
		Logger::notice("two way interactor", pubfeed.c_str());
		//	Logger::notice("two way interactor",this->getSubfeed());
	}

	SEFL::MQTT_Subscriber_Object_Bound<MQTT_Two_Way_Interactor> *MQTT_Two_Way_Interactor::getSub()
	{
		return (&sub_);
	}

	String MQTT_Two_Way_Interactor::getSubfeed()
	{
		return (subfeed_);
	}

	MQTT_Two_Way_Interactor::~MQTT_Two_Way_Interactor()
	{
		// TODO Auto-generated destructor stub
	}

} /* namespace SEFL */

/*
 * questclient.cpp
 *
 *  Created on: May 16, 2021
 *      Author: vladi
 */

#include <utils/logger.h>
#include <quest_clients/questclient.h>

#include <MQTT.h>
namespace SEFL
{

	Quest_Client::~Quest_Client()
	{
		// TODO Auto-generated destructor stub
	}

	bool Quest_Client::isChangedStatus()
	{
		return (changed_status_);
	}

	Array<String, MAX_DATA_LENGTH> &Quest_Client::getData()
	{
		return (data);
	}

	SEFL::Language Quest_Client::getLanguage()
	{
		return (language_);
	}

	void Quest_Client::setLanguage(SEFL::Language language_)
	{
		this->language_ = language_;
	}

	const char *Quest_Client::getName()
	{
		return (name_);
	}

	bool Quest_Client::isPowerStatus()
	{
		return (power_status_);
	}

	void Quest_Client::setPowerStatus(bool power_status_)
	{
		this->power_status_ = power_status_;
	}

	void Quest_Client::unsetChangedStatus()
	{
		this->changed_status_ = false;
	}

	void Quest_Client::cleanData()
	{
		this->data.clear();
	}

	void Quest_Client::setChangedStatus(bool changedStatus)
	{
		this->changed_status_ = changedStatus;
	}

	Quest_Client::Quest_Client(MQTTClient &mqtt, const char *name, ClientType type,
							   uint8_t reset_status, const char *placement, const char *in_topic,
							   const char *out_topic, SEFL::Language language) : MQTT_Two_Way_Interactor(mqtt,
																										 String(String(placement) + TOPIC_DELIMETER + name + TOPIC_DELIMETER + in_topic),
																										 String(String(placement) + TOPIC_DELIMETER + name + TOPIC_DELIMETER + out_topic)),
																				 changed_status_(
																					 true),
																				 name_(name), type_(type), power_status_(true), language_(language), reset_status_(
																																						 reset_status)
	{
		this->data.clear();

		Logger::notice("quest client", F("constructor_done"));
	}

	const char *Quest_Client::asString(ClientType client)
	{
		return (CLIENT_STRINGS[client]);
	}

	ClientType Quest_Client::getType()
	{
		return (type_);
	}

} /* namespace SEFL */

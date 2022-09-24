/*
 * home.cpp
 *
 *  Created on: Sep 18, 2022
 *      Author: piatk
 */

#include <custom_clients/home.h>
#include <constants.h>
#include <utils/logger.h>
#include <sys/_stdint.h>

namespace SEFL
{

	Home::Home(MQTTClient &mqtt, const char *name, uint8_t reset_status,
			   const char *placement, const char *in_topic, const char *out_topic,
			   SEFL::Language language) : Quest_Basic_Client(mqtt, name, reset_status, placement, in_topic,
															 out_topic, language)
	{
		count = 0;
	}

	Home::~Home()
	{
		// TODO Auto-generated destructor stub
	}

	void Home::onActive()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("home", "onActive");
		}
		if (count == 0)
		{
			count++;
			this->setStatus(BasicClientStatuses::FINISHED_STATUS);
		}
	}

	void Home::onFinished()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("home", "onFinished");
		}
	}

	void Home::onManualFinished()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("home", "onManualFinished");
		}
	}

	uint16_t Home::getCount()
	{
		return count;
	}

	void Home::setCount(uint16_t count)
	{
		this->count = count;
	}

	void Home::onDefault()
	{
		if (isChangedStatus())
		{
			unsetChangedStatus();
			this->reportStatus();
			SEFL::Logger::verbose("home", "onDefault");
			count = 0;
		}
	}

} /* namespace SEFL */

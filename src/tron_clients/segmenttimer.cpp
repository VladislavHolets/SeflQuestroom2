/*
 * tronsegmenttimer.cpp
 *
 *  Created on: Aug 16, 2021
 *      Author: vladi
 */

#include "segmenttimer.h"

namespace SEFL
{

	Tron_Segment_Timer::Tron_Segment_Timer(MQTTClient &mqtt, HC595_cfg cfg, const char *name,
										   uint8_t reset_status, const char *placement,
										   const char *in_topic, const char *out_topic, SEFL::Language language) : Quest_Basic_Client(mqtt, name, reset_status, placement, in_topic,
																																	  out_topic, language),
																												   cfg_(cfg)
	{

		// TODO Auto-generated constructor stub
	}

	Tron_Segment_Timer::~Tron_Segment_Timer()
	{
		// TODO Auto-generated destructor stub
	}

	void Tron_Segment_Timer::onActive()
	{
	}

	void Tron_Segment_Timer::onFinished()
	{
	}

	void Tron_Segment_Timer::onManualFinished()
	{
	}

	void Tron_Segment_Timer::onDefault()
	{
	}

} /* namespace SEFL */

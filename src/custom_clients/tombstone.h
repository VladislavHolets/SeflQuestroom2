/*
 * tombstone.h
 *
 *  Created on: Jan 25, 2022
 *      Author: piatk
 */

#ifndef TOMBSTONE_H_
#define TOMBSTONE_H_

#include <quest_clients/questbasicclient.h>

namespace SEFL
{

	class Tombstone : public Quest_Basic_Client
	{
		uint8_t magnet_tombstone, motojacket_button;
		bool inverted;

	public:
		Tombstone(MQTTClient &mqtt, const char *name = DEFAULT_DEVICE_NAME,
				  uint8_t reset_status = 1, const char *placement = DEFAULT_PLACEMENT,
				  const char *in_topic = DEFAUT_IN_TOPIC, const char *out_topic = DEFAUT_OUT_TOPIC, SEFL::Language language = UKR);
		virtual ~Tombstone();
		virtual void onActive() override;
		virtual void onFinished() override;
		virtual void onManualFinished() override;
		virtual void onDefault() override;
		bool isInverted();
		void setInverted(bool inverted);
		uint8_t getMagnetTombstone();
		void setMagnetTombstone(uint8_t magnetTombstone);
		uint8_t getMotojacketButton();
		void setMotojacketButton(uint8_t motojacketButton);
	};

} /* namespace SEFL */

#endif /* TOMBSTONE_H_ */

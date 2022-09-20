/*
 * queststartbutton.h
 *
 *  Created on: May 17, 2021
 *      Author: vladi
 */

#ifndef QUESTSTARTBUTTON_H_
#define QUESTSTARTBUTTON_H_

#include <constants.h>
#include <quest_clients/questclient.h>
#include <sys/_stdint.h>

namespace SEFL
{

	class Quest_Start_Button : public Quest_Client
	{
	protected:
		uint8_t start_pin_;
		SEFL::StartButtonClientStatuses status_;

	public:
		Quest_Start_Button(MQTT &mqtt, uint8_t start_pin, const char *name = DEFAULT_DEVICE_NAME, uint8_t reset_status = 1,
						   const char *placement = DEFAULT_PLACEMENT, const char *in_topic = DEFAUT_IN_TOPIC, const char *out_topic = DEFAUT_OUT_TOPIC,
						   SEFL::Language language = UKR);
		virtual ~Quest_Start_Button();
		virtual void act() override;
		virtual void inputClb(const char *data, uint16_t len) override;
		virtual void setStatus(uint8_t status) override;
		virtual void reportStatus() override;
		virtual void onDefault();
		virtual void onActivated();
		virtual void onDisabled();
		uint8_t getStartPin() const;
		SEFL::StartButtonClientStatuses getStatus() const;
	};

} /* namespace SEFL */

#endif /* QUESTSTARTBUTTON_H_ */

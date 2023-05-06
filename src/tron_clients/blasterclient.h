/*
 * BlasterClient.h
 *
 *  Created on: Jul 2, 2021
 *      Author: vladi
 */
#if defined(ARDUINO_ARCH_ESP32)
#ifndef BLASTERCLIENT_H_
#define BLASTERCLIENT_H_

#include <cstdint>

#include "sefl_quest_core.h"

class DFRobotDFPlayerMini;

namespace SEFL {
enum BlasterClientStatuses {
	DISABLED_STATUS = 1, DISARMED_STATUS = 2, ARMED_STATUS = 3
};
enum Tracks {
	FIRE_FAILED_SOUND = 1, FIRE_SOUND = 2, ARMED_SOUND = 3, DISARMED_SOUND = 4
};
enum BlasterEnum{BLASTER_INF_BULLETS=-1};

class Blaster_Client: public Quest_Client {
	int8_t bullets;
	bool press_registred;
    int16_t button_pin;
    int16_t transmitter_pin;
    int16_t pointer_pin;
	DFRobotDFPlayerMini *player_;
public:
    void setButtonPin(int16_t buttonPin);

    void setPlayer(DFRobotDFPlayerMini &player);

    void setTransmitterPin(int16_t transmitterPin);

    void setPointerPin(int16_t pointerPin);

    SEFL::BlasterClientStatuses status_;
	Blaster_Client(MQTTClient &mqtt,const char *name = DEFAULT_DEVICE_NAME,
			uint8_t reset_status = 1, const char *placement = DEFAULT_PLACEMENT,
			const char *in_topic = DEFAUT_IN_TOPIC, const char *out_topic =
					DEFAUT_OUT_TOPIC, SEFL::Language language = UKR);
	virtual ~Blaster_Client();
	virtual void inputClb(const char *data, uint16_t len) override;
	virtual void act() override;
	virtual void setStatus(uint8_t status) override;
	virtual void reportStatus() override;
	SEFL::BlasterClientStatuses getStatus() const;
	virtual void onDefault();
	virtual void onDisarmed();
	virtual void onArmed();
};

} /* namespace SEFL */

#endif /* BLASTERCLIENT_H_ */
#endif
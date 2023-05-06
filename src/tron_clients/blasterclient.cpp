/*
 * BlasterClient.cpp
 *
 *  Created on: Jul 2, 2021
 *      Author: vladi
 */
#if defined(ARDUINO_ARCH_ESP32)
#include "BlasterClient.h"


#include <ArduinoJson.h>
#include <Array/ArrayDefinitions.h>
#include <Array.h>
#include <DFRobotDFPlayerMini.h>
#include <esp32-hal.h>
#include <esp32-hal-gpio.h>
#include <IRremote.h>
#include <WString.h>


namespace SEFL {

Blaster_Client::~Blaster_Client() {
	// TODO Auto-generated destructor stub
}

void Blaster_Client::inputClb(const char *data, uint16_t len) {
	DynamicJsonDocument doc(SEFL::DOC_SIZE);
	deserializeJson(doc, data, len);
	JsonArray arr = doc["Data"].as<JsonArray>();
	for (JsonVariant value : arr) {
		this->data.push_back(String(value.as<const char*>()));
	}
	//	}
	int command = doc["CommandId"];
	switch (command) {
	case SEFL::DirectCommands::PING_COMMAND: {
		this->getMqtt()->publish(this->getPubfeed().c_str(), data, 1);
	}
		break;
	case SEFL::DirectCommands::STATUS_COMMAND: {

		char output[128];
		DynamicJsonDocument repDoc(SEFL::DOC_SIZE);
		repDoc["CommandId"] =
				static_cast<int>(SEFL::DirectCommands::STATUS_COMMAND);
		int tstatus = static_cast<int>(this->getStatus());
		repDoc["SubcommandId"] = tstatus;
		JsonArray jdata = repDoc.createNestedArray("Data");
		serializeJson(repDoc, output);
		this->getMqtt()->publish(this->getPubfeed().c_str(), output, 1);
	}
		break;
	case SEFL::DirectCommands::ACTION_COMMAND: {
		this->setPowerStatus(true);
		enum SEFL::BlasterClientStatuses status =
				static_cast<SEFL::BlasterClientStatuses>(doc["SubcommandId"].as<
						int>());
		this->setStatus(status);

		this->getMqtt()->publish(this->getPubfeed().c_str(), data, 1);
	}
		break;
	case SEFL::DirectCommands::DEACTIVATE_DEVICE_COMMAND: {
		this->setPowerStatus(false);
	}
		break;
	case SEFL::DirectCommands::DEBUG_MODE_COMMAND: {
		//	this->setPowerStatus(false);
	}
		break;
	case SEFL::DirectCommands::RESET_COMMAND: {
		this->setStatus((this->reset_status_));
		this->cleanData();
	}
		break;
	default: {
	}
	}
}

void Blaster_Client::act() {
	switch (this->getStatus()) {
	case SEFL::BlasterClientStatuses::DISABLED_STATUS: {
		this->onDefault();

	}
		break;
	case SEFL::BlasterClientStatuses::DISARMED_STATUS: {
		this->onDisarmed();
	}
		break;
	case SEFL::BlasterClientStatuses::ARMED_STATUS: {
		this->onArmed();
	}
		break;
	}
}

void Blaster_Client::setStatus(uint8_t status) {
	if (this->status_ != static_cast<SEFL::BlasterClientStatuses>(status)) {
		status_ = static_cast<SEFL::BlasterClientStatuses>(status);
		this->changed_status_ = true;
	}
}

Blaster_Client::Blaster_Client(MQTTClient &mqtt,
		const char *name, uint8_t reset_status, const char *placement,
		const char *in_topic, const char *out_topic, SEFL::Language language) :
        Quest_Client(mqtt, name, TRON_BLASTER, reset_status, placement,
				in_topic, out_topic, language), bullets(0), press_registred(0), status_(
				SEFL::BlasterClientStatuses::DISABLED_STATUS) {
    transmitter_pin=-1;
    button_pin=-1;
    pointer_pin=-1;
    player_= nullptr;
}

void Blaster_Client::reportStatus() {
	char output[128];
	DynamicJsonDocument repDoc(SEFL::DOC_SIZE);
	repDoc["CommandId"] =
			static_cast<int>(SEFL::DirectCommands::STATUS_TRIGGER_COMMAND);
	int tstatus = static_cast<int>(this->getStatus());
	repDoc["SubcommandId"] = tstatus;
	JsonArray jdata = repDoc.createNestedArray("Data");
	for (auto & i : this->data) {
		if (i.length())
			jdata.add(i);
	}
	serializeJson(repDoc, output);
	this->getMqtt()->publish(this->getPubfeed().c_str(), output, 1);
}

SEFL::BlasterClientStatuses Blaster_Client::getStatus() const {
	return (status_);
}

void Blaster_Client::onDefault() {
	if (isChangedStatus()) {
		unsetChangedStatus();
		reportStatus();
        if(pointer_pin==-1 || transmitter_pin==-1 || button_pin==-1){
            return;
        }
		digitalWrite(pointer_pin, LOW);
	}
}

void Blaster_Client::onDisarmed() {
	if (isChangedStatus()) {
		this->data.clear();
		unsetChangedStatus();
		reportStatus();
        if(player_!= nullptr)
		    player_->play(Tracks::DISARMED_SOUND);
        if(pointer_pin==-1 || transmitter_pin==-1 || button_pin==-1){
            return;
        }
		digitalWrite(pointer_pin, LOW);
	}
    if(pointer_pin==-1 || transmitter_pin==-1 || button_pin==-1){
        return;
    }
	if (digitalRead(button_pin) == false && press_registred == false) {
		press_registred = true;

        if(player_!= nullptr)
		    player_->play(Tracks::FIRE_FAILED_SOUND);
		delay(50);
	}
	if (digitalRead(button_pin) == true && press_registred == true) {
		press_registred = false;
	}

}

void Blaster_Client::onArmed() {
	if (isChangedStatus()) {
		unsetChangedStatus();
		reportStatus();
        if(player_!= nullptr)
		    player_->play(Tracks::ARMED_SOUND);
        if(pointer_pin==-1 || transmitter_pin==-1 || button_pin==-1){
            return;
        }
		digitalWrite(pointer_pin, HIGH);
	}
    if(pointer_pin==-1 || transmitter_pin==-1 || button_pin==-1){
        return;
    }
	SEFL::Logger::notice(this->name_,"armed");
	SEFL::Logger::notice(this->name_,bullets);

	while (!this->data.empty()) {
		if (this->data.front().toInt() == BLASTER_INF_BULLETS) {
			this->bullets = -1;
			this->data.remove(0);
		} else if (this->bullets != -1
				&& this->data.back().toInt() != BLASTER_INF_BULLETS) {
			bullets += this->data.front().toInt();
			this->data.remove(0);
		}
	}
	if (bullets == 0) {
		this->setStatus(SEFL::BlasterClientStatuses::DISARMED_STATUS);
	}
	if (digitalRead(button_pin) == true && press_registred == false) {
		press_registred = true;
        if(player_!= nullptr)
		    player_->play(Tracks::FIRE_SOUND);
		delay(50);
		IrSender.sendNECRaw(3016);
		if (bullets > 0) {
			bullets -= 1;
		}
	}
	if (digitalRead(button_pin) == false && press_registred == true) {
		press_registred = false;
	}
}

    void Blaster_Client::setButtonPin(int16_t buttonPin) {
    if(button_pin!=-1){
        pinMode(button_pin,INPUT);
    }
        button_pin = buttonPin;
        pinMode(button_pin,INPUT_PULLDOWN);

    }

    void Blaster_Client::setTransmitterPin(int16_t transmitterPin) {
        if(transmitter_pin!=-1){
            pinMode(transmitter_pin,INPUT);
        }
        transmitter_pin = transmitterPin;
        IrSender.begin(transmitter_pin);
    }

    void Blaster_Client::setPointerPin(int16_t pointerPin) {
        if(pointer_pin!=-1){
            pinMode(pointer_pin,INPUT);
        }
        pointer_pin = pointerPin;
        pinMode(pointer_pin, OUTPUT);
        digitalWrite(pointer_pin, LOW);
    }

    void Blaster_Client::setPlayer(DFRobotDFPlayerMini &player) {
        player_ = &player;
    }
} /* namespace SEFL */
#endif
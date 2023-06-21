/*
 * Constants.h
 *
 *  Created on: Sep 13, 2020
 *      Author: vladi
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <sys/_stdint.h>
#if defined(ARDUINO_ARCH_STM32)
#include <wiring_constants.h>
#endif
#include <Wire.h>

#if defined(ARDUINO_ARCH_AVR)
#include <avr/pgmspace.h>
#endif

// There appears to be an incompatibility with ESP8266 2.3.0.
#if defined(ESP8266)
#define MEM_TYPE
#else
#define MEM_TYPE PROGMEM
#endif
namespace SEFL
{
	static const uint8_t MAX_DATA_LENGTH = 10;
	static const uint32_t shutdown_timeout = 60000;
	enum BroadcastComands
	{
		REQUEST_CONFIG_COMMAND = 1,
		LOCALIZATION_SETUP_COMMAND = 2,
		ENABLE_POWER_COMMAND = 3,
		DISABLE_POWER_COMMAND = 4
	};
	enum DirectCommands
	{
		PING_COMMAND = 1,
		STATUS_TRIGGER_COMMAND = 2,
		STATUS_COMMAND = 12,
		ACTION_COMMAND = 3,
		RESET_COMMAND = 4,
		DEACTIVATE_DEVICE_COMMAND = 5,
		DEBUG_MODE_COMMAND = 6
	};

	static const uint32_t CALLBACK_TIMEOUT = 60000;
    static const uint16_t MAX_MESSAGE_QUEUE_AMOUNT = 500;
    static const uint16_t MAX_DELAYED_MESSAGE_QUEUE_AMOUNT = 50;
	static const uint8_t MAX_CLIENT_AMOUNT = 50;
	static const char DEFAULT_DEVICE_TYPE[] MEM_TYPE = "DEFAULT";
	static const char ACTUATOR_DEVICE_TYPE[] MEM_TYPE = "Actuator";
	static const char BASIC_DEVICE_TYPE[] MEM_TYPE = "Basic";
	static const char SENSOR_DEVICE_TYPE[] MEM_TYPE = "Sensor";
	static const char HOST_DEVICE_TYPE[] MEM_TYPE = "Host";
	static const char START_BUTTON_DEVICE_TYPE[] MEM_TYPE = "StartButton";
	static const char TIMER_DEVICE_TYPE[] MEM_TYPE = "Timer";
	static const char MEDIA_PLAYER_DEVICE_TYPE[] MEM_TYPE = "MediaPlayer";
	static const char QUEST_CONTROL_SERVER_DEVICE_TYPE[] MEM_TYPE = "Master";

	enum ClientType
	{
		DEFAULT_DEVICE = 0,
		ACTUATOR = 1,
		BASIC = 2,
		SENSOR = 3,
		START_BUTTON = 4,
		TIMER = 5,
		MEDIA_PLAYER = 6,
		SERVER = 7,
		HOST = 8,
        TRON_BLASTER=12,
		TRON_TARGET = 13,
	};
	static const char *const CLIENT_STRINGS[] MEM_TYPE = {
		DEFAULT_DEVICE_TYPE,
		ACTUATOR_DEVICE_TYPE,
		BASIC_DEVICE_TYPE,
		SENSOR_DEVICE_TYPE,
		START_BUTTON_DEVICE_TYPE,
		TIMER_DEVICE_TYPE,
		MEDIA_PLAYER_DEVICE_TYPE,
		QUEST_CONTROL_SERVER_DEVICE_TYPE,
		HOST_DEVICE_TYPE,
	};

	enum BasicClientStatuses
	{
		DEFAULT_STATUS = 1,
		ACTIVE_STATUS = 2,
		FINISHED_STATUS = 3,
		MANUAL_FINISHED_STATUS = 4
	};

	enum StartButtonClientStatuses
	{
		ACTIVATED_BUTTON_STATUS = 1,
		DEFAULT_BUTTON_STATUS = 2,
		DISABLED_BUTTON_STATUS = 3

	};

	enum ActuatorClientStatuses
	{
		OFF_STATUS = 1,
		ON_STATUS = 2
	};

	enum SensorClientStatuses
	{
		SET_STATUS = 1,
		RESET_STATUS = 2
	};

	// reseted as basic device(reset comand and with action command),
	// if in reset state being triggered by hardware sensors - goes to SET and sends a STATUS_TRIGGER_COMMAND
	enum TimerClientStatuses
	{
		FULL_STATUS = 1,
		ENABLED_STATUS = 2,
		FREE_STATUS = 3,
		TRIGGERED_STATUS = 4
	};

	enum TimerClientCommands
	{
		SET = 1,
		UNSET = 2
	};

	enum BasicClientActionSubcommands
	{
		ACTIVATE = 1,
		DEACTIVATE = 2,
		SOLVE = 3
	};
	enum Language
	{
		ENG = 2,
		UKR = 1,
		RUS = 0
	};
	static const uint16_t DOC_SIZE = 512;
	static const char DEFAUT_OUT_TOPIC[] = "out";
	static const char DEFAUT_IN_TOPIC[] = "in";
	static const char DEFAUT_DEBUG_TOPIC[] = "debug";
	static const char DEFAULT_PLACEMENT[] = "default";
	static const char DEFAULT_SERVER_NAME[] = "server";
	static const char DEFAULT_DEVICE_NAME[] = "device";
	static const char TOPIC_DELIMETER[] = "/";
	static const bool DEFAULT_POWER_STATUS = true;
	static const Language DEFAULT_LANGUAGE = SEFL::Language::UKR;

	struct MQTT_Config
	{
		const char *IP;
		uint16_t port;
		const char *username;
		const char *password;
	};
	struct ExpansionPort
	{
		uint8_t RESET_PIN;
		uint8_t MANUAL_1_PIN;
		uint8_t MANUAL_2_PIN;
		uint8_t MANUAL_3_PIN;
	};

	static const MQTT_Config RYBALSKA3_MQTT_CONFIG = {"192.168.90.132", 1883,
													  "questroom", "likekadroom"};

	static const MQTT_Config EDMONTON_MQTT_CONFIG = {"192.168.10.10", 1883,
													 "questroom", "likekadroom"};
	static const uint8_t QOS_DEFAULT = 2;
}
#endif /* CONSTANTS_H_ */

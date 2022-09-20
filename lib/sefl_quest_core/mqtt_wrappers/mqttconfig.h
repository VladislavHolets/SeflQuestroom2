/*
 * mqttconfig.h
 *
 *  Created on: May 4, 2021
 *      Author: vladi
 */

#ifndef MQTTCONFIG_H_
#define MQTTCONFIG_H_

#if defined(ARDUINO_SAMD_ZERO) || defined(ARDUINO_STM32_FEATHER)
#define strncpy_P(dest, src, len) strncpy((dest), (src), (len))
#define strncasecmp_P(f1, f2, len) strncasecmp((f1), (f2), (len))
#endif

const auto ADAFRUIT_MQTT_VERSION_MAJOR = 0;
const auto ADAFRUIT_MQTT_VERSION_MINOR = 17;
const auto ADAFRUIT_MQTT_VERSION_PATCH = 0;

// Use 3 (MQTT 3.0) or 4 (MQTT 3.1.1)
const auto MQTT_PROTOCOL_LEVEL = 4;

const auto MQTT_CTRL_CONNECT = 0x1;
const auto MQTT_CTRL_CONNECTACK = 0x2;
const auto MQTT_CTRL_PUBLISH = 0x3;
const auto MQTT_CTRL_PUBACK = 0x4;
const auto MQTT_CTRL_PUBREC = 0x5;
const auto MQTT_CTRL_PUBREL = 0x6;
const auto MQTT_CTRL_PUBCOMP = 0x7;
const auto MQTT_CTRL_SUBSCRIBE = 0x8;
const auto MQTT_CTRL_SUBACK = 0x9;
const auto MQTT_CTRL_UNSUBSCRIBE = 0xA;
const auto MQTT_CTRL_UNSUBACK = 0xB;
const auto MQTT_CTRL_PINGREQ = 0xC;
const auto MQTT_CTRL_PINGRESP = 0xD;
const auto MQTT_CTRL_DISCONNECT = 0xE;

const auto MQTT_QOS_1 = 0x1;
const auto MQTT_QOS_0 = 0x0;

const auto CONNECT_TIMEOUT_MS = 6000;
const auto PUBLISH_TIMEOUT_MS = 500;
const auto PING_TIMEOUT_MS = 20;
const auto SUBACK_TIMEOUT_MS = 500;

// Adjust as necessary, in seconds.  Default to 5 minutes.
const auto MQTT_CONN_KEEPALIVE = 30000;

// Largest full packet we're able to send.
// Need to be able to store at least ~90 chars for a connect packet with full
// 23 char client ID.
const auto MAXBUFFERSIZE = (500);

const auto MQTT_CONN_USERNAMEFLAG = 0x80;
const auto MQTT_CONN_PASSWORDFLAG = 0x40;
const auto MQTT_CONN_WILLRETAIN = 0x20;
const auto MQTT_CONN_WILLQOS_1 = 0x08;
const auto MQTT_CONN_WILLQOS_2 = 0x18;
const auto MQTT_CONN_WILLFLAG = 0x04;
const auto MQTT_CONN_CLEANSESSION = 0x02;

// how much data we save in a subscription object
// and how many subscriptions we want to be able to track.
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega328P__)
const auto  MAXSUBSCRIPTIONS =5;
const auto  SUBSCRIPTIONDATALEN =20;
#else
const auto MAXSUBSCRIPTIONS = 20;
const auto SUBSCRIPTIONDATALEN = 100;
#endif

#endif /* MQTTCONFIG_H_ */

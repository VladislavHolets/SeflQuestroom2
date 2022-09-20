/*
 * mqttmanager.h
 *
 *  Created on: May 4, 2021
 *      Author: vladi
 */

#ifndef MQTT_H_
#define MQTT_H_

#include <mqtt_wrappers/mqttconfig.h>
#include <sys/_stdint.h>

namespace SEFL
{
	class MQTT_Subscriber;
} /* namespace SEFL */

class __FlashStringHelper;

namespace SEFL
{

	class MQTT
	{
	private:
		SEFL::MQTT_Subscriber *subscriptions[MAXSUBSCRIPTIONS];
		uint8_t *stringprint(uint8_t *p, const char *s, uint16_t maxlen = 0);
		void flushIncoming(uint32_t timeout);

		// Functions to generate MQTT packets.
		uint8_t connectPacket(uint8_t *packet);
		uint8_t disconnectPacket(uint8_t *packet);
		uint16_t publishPacket(uint8_t *packet, const char *topic, uint8_t *payload,
							   uint16_t bLen, uint8_t qos);
		uint8_t subscribePacket(uint8_t *packet, const char *topic, uint8_t qos);
		uint8_t unsubscribePacket(uint8_t *packet, const char *topic);
		uint8_t pingPacket(uint8_t *packet);
		uint8_t pubackPacket(uint8_t *packet, uint16_t packetid);

	protected:
		// Interface that subclasses need to implement:

		// Connect to the server and return true if successful, false otherwise.
		virtual bool connectServer() = 0;

		// Disconnect from the MQTT server.  Returns true if disconnected, false
		// otherwise.
		virtual bool disconnectServer() = 0; // Subclasses need to fill this in!

		// Send data to the server specified by the buffer and length of data.
		virtual bool sendPacket(uint8_t *buffer, uint16_t len) = 0;

		// Read MQTT packet from the server.  Will read up to maxlen bytes and store
		// the data in the provided buffer.  Waits up to the specified timeout (in
		// milliseconds) for data to be available.
		virtual uint16_t readPacket(uint8_t *buffer, uint16_t maxlen,
									uint32_t timeout) = 0;

		// Read a full packet, keeping note of the correct length
		uint16_t readFullPacket(uint8_t *buffer, uint16_t maxsize,
								uint32_t timeout);
		// Properly process packets until you get to one you want
		uint16_t processPacketsUntil(uint8_t *buffer, uint8_t waitforpackettype,
									 uint32_t timeout);

		// Shared state that subclasses can use:
		const char *servername;
		int16_t portnum;
		const char *clientid;
		const char *username;
		const char *password;
		const char *will_topic;
		const char *will_payload;
		uint8_t will_qos;
		uint8_t will_retain;
		uint8_t buffer[MAXBUFFERSIZE]; // one buffer, used for all incoming/outgoing
		uint16_t packet_id_counter;
		const char *logger_module_name;

	public:
		MQTT(const char *server, uint16_t port, const char *cid,
			 const char *user, const char *pass, const char *logger_module = "mqtt_manager");
		MQTT(const char *server, uint16_t port, const char *user = "",
			 const char *pass = "", const char *logger_module = "mqtt_manager");
		virtual ~MQTT();
		// Connect to the MQTT server.  Returns 0 on success, otherwise an error code
		// that indicates something went wrong:
		//   -1 = Error connecting to server
		//    1 = Wrong protocol
		//    2 = ID rejected
		//    3 = Server unavailable
		//    4 = Bad username or password
		//    5 = Not authenticated
		//    6 = Failed to subscribe
		// Use connectErrorString() to get a printable string version of the
		// error.
		int8_t connect();
		int8_t connect(const char *user, const char *pass);

		// Return a printable string version of the error code returned by
		// connect(). This returns a __FlashStringHelper*, which points to a
		// string stored in flash, but can be directly passed to e.g.
		// Serial.println without any further processing.
		const __FlashStringHelper *connectErrorString(int8_t code);

		// Sends MQTT disconnect packet and calls disconnectServer()
		bool disconnect();

		// Return true if connected to the MQTT server, otherwise false.
		virtual bool connected() = 0; // Subclasses need to fill this in!

		// Set MQTT last will topic, payload, QOS, and retain. This needs
		// to be called before connect() because it is sent as part of the
		// connect control packet.
		bool will(const char *topic, const char *payload, uint8_t qos = 0,
				  uint8_t retain = 0);

		// Publish a message to a topic using the specified QoS level.  Returns true
		// if the message was published, false otherwise.
		bool publish(const char *topic, const char *payload, uint8_t qos = 0);
		bool publish(const char *topic, uint8_t *payload, uint16_t bLen,
					 uint8_t qos = 0);

		// Add a subscription to receive messages for a topic.  Returns true if the
		// subscription could be added or was already present, false otherwise.
		// Must be called before connect(), subscribing after the connection
		// is made is not currently supported.
		bool subscribe(MQTT_Subscriber *sub);

		// Unsubscribe from a previously subscribed MQTT topic.
		bool unsubscribe(MQTT_Subscriber *sub);

		// Check if any subscriptions have new messages.  Will return a reference to
		// an Adafruit_MQTT_Subscribe object which has a new message.  Should be
		// called in the sketch's loop function to ensure new messages are recevied.
		// Note that subscribe should be called first for each topic that receives
		// messages!
		MQTT_Subscriber *readSubscription(uint32_t timeout = 0);

		bool processPackets(uint32_t timeout);

		// Ping the server to ensure the connection is still alive.
		bool ping(uint8_t n = 1);
	};

} /* namespace SEFL */

#endif /* MQTT_H_ */

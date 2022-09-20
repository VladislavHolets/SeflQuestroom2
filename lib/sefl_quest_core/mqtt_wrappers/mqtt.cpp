/*
 * mqttmanager.cpp
 *
 *  Created on: May 4, 2021
 *      Author: vladi
 */

#include <pgmspace.h>
#include <utils/logger.h>
#include <mqtt_wrappers/mqtt.h>
#include <mqtt_wrappers/mqttsubscriber.h>
#include <strings.h>
#include <wiring_time.h>
#include <WString.h>
#include <cstring>

namespace SEFL
{

#if defined(ARDUINO_SAMD_ZERO) || defined(ARDUINO_SAMD_MKR1000) || \
	defined(ARDUINO_ARCH_SAMD)
	static char *dtostrf(double val, signed char width, unsigned char prec,
						 char *sout)
	{
		char fmt[20];
		sprintf(fmt, "%%%d.%df", width, prec);
		sprintf(sout, fmt, val);
		return sout;
	}
#endif

#if defined(ESP8266)
	int strncasecmp(const char *str1, const char *str2, int len)
	{
		int d = 0;
		while (len--)
		{
			int c1 = tolower(*str1++);
			int c2 = tolower(*str2++);
			if (((d = c1 - c2) != 0) || (c2 == '\0'))
			{
				return d;
			}
		}
		return 0;
	}
#endif

	uint8_t *MQTT::stringprint(uint8_t *p, const char *s, uint16_t maxlen)
	{
		// If maxlen is specified (has a non-zero value) then use it as the maximum
		// length of the source string to write to the buffer.  Otherwise write
		// the entire source string.
		uint16_t len = strlen(s);
		if (maxlen > 0 && len > maxlen)
		{
			len = maxlen;
		}
		/*
		 for (uint8_t i=0; i<len; i++) {
		 Serial.write(pgm_read_byte(s+i));
		 }
		 */
		p[0] = len >> 8;
		p++;
		p[0] = len & 0xFF;
		p++;
		strncpy(reinterpret_cast<char *>(p), s, len);
		return (p + len);
	}

	void MQTT::flushIncoming(uint32_t timeout)
	{
		// flush input!
		Logger::warning(this->logger_module_name, F("Flushing input buffer"));
		while (readPacket(buffer, MAXBUFFERSIZE, timeout))
			;
	}

	uint8_t MQTT::connectPacket(uint8_t *packet)
	{
		uint8_t *p = packet;
		uint16_t len;

		// fixed header, connection messsage no flags
		p[0] = (MQTT_CTRL_CONNECT << 4) | 0x0;
		p += 2;
		// fill in packet[1] last

		if (MQTT_PROTOCOL_LEVEL == 3)
			p = stringprint(p, "MQIsdp");
		else if (MQTT_PROTOCOL_LEVEL == 4)
			p = stringprint(p, "MQTT");
		else
		{
			Logger::error(this->logger_module_name, F("MQTT level not supported"));
		}

		p[0] = MQTT_PROTOCOL_LEVEL;
		p++;

		// always clean the session
		p[0] = MQTT_CONN_CLEANSESSION;

		// set the will flags if needed
		if (will_topic && pgm_read_byte(will_topic) != 0)
		{

			p[0] |= MQTT_CONN_WILLFLAG;

			if (will_qos == 1)
				p[0] |= MQTT_CONN_WILLQOS_1;
			else if (will_qos == 2)
				p[0] |= MQTT_CONN_WILLQOS_2;

			if (will_retain == 1)
				p[0] |= MQTT_CONN_WILLRETAIN;
		}

		if (strlen(username) != 0)
			p[0] |= MQTT_CONN_USERNAMEFLAG;
		if (strlen(password) != 0)
			p[0] |= MQTT_CONN_PASSWORDFLAG;
		p++;

		p[0] = MQTT_CONN_KEEPALIVE >> 8;
		p++;
		p[0] = MQTT_CONN_KEEPALIVE & 0xFF;
		p++;

		if (MQTT_PROTOCOL_LEVEL == 3)
		{
			p = stringprint(p, clientid, 23); // Limit client ID to first 23 characters.
		}
		else
		{
			if (pgm_read_byte(clientid) != 0)
			{
				p = stringprint(p, clientid);
			}
			else
			{
				p[0] = 0x0;
				p++;
				p[0] = 0x0;
				p++;
				Logger::warning(this->logger_module_name,
								"SERVER GENERATING CLIENT ID");
			}
		}

		if (will_topic && pgm_read_byte(will_topic) != 0)
		{
			p = stringprint(p, will_topic);
			p = stringprint(p, will_payload);
		}

		if (strlen(username) != 0)
		{
			p = stringprint(p, username, strlen(username));
		}
		if (strlen(password) != 0)
		{
			p = stringprint(p, password, strlen(password));
		}

		len = p - packet;

		packet[1] = len - 2; // don't include the 2 bytes of fixed header data
		Logger::warning(this->logger_module_name, F("MQTT connect packet:"));
		Logger::warning(this->logger_module_name, buffer, len);
		return (len);
	}

	uint8_t MQTT::disconnectPacket(uint8_t *packet)
	{
		packet[0] = MQTT_CTRL_DISCONNECT << 4;
		packet[1] = 0;
		Logger::warning(this->logger_module_name, F("MQTT disconnect packet:"));
		Logger::warning(this->logger_module_name, buffer, 2);
		return (2);
	}

	uint16_t MQTT::publishPacket(uint8_t *packet, const char *topic,
								 uint8_t *payload, uint16_t bLen, uint8_t qos)
	{
		uint8_t *p = packet;
		uint16_t len = 0;

		// calc length of non-header data
		len += 2;			  // two bytes to set the topic size
		len += strlen(topic); // topic length
		if (qos > 0)
		{
			len += 2; // qos packet id
		}
		len += bLen; // payload length

		// Now you can start generating the packet!
		p[0] = MQTT_CTRL_PUBLISH << 4 | qos << 1;
		p++;

		// fill in packet[1] last
		do
		{
			uint8_t encodedByte = len % 128;
			len /= 128;
			// if there are more data to encode, set the top bit of this byte
			if (len > 0)
			{
				encodedByte |= 0x80;
			}
			p[0] = encodedByte;
			p++;
		} while (len > 0);

		// topic comes before packet identifier
		p = stringprint(p, topic);

		// add packet identifier. used for checking PUBACK in QOS > 0
		if (qos > 0)
		{
			p[0] = (packet_id_counter >> 8) & 0xFF;
			p[1] = packet_id_counter & 0xFF;
			p += 2;

			// increment the packet id
			packet_id_counter++;
		}

		memmove(p, payload, bLen);
		p += bLen;
		len = p - packet;
		Logger::warning(this->logger_module_name, F("MQTT publish packet:"));
		Logger::warning(this->logger_module_name, buffer, len);
		return (len);
	}

	uint8_t MQTT::subscribePacket(uint8_t *packet, const char *topic,
								  uint8_t qos)
	{
		uint8_t *p = packet;
		uint16_t len;

		p[0] = MQTT_CTRL_SUBSCRIBE << 4 | MQTT_QOS_1 << 1;
		// fill in packet[1] last
		p += 2;

		// packet identifier. used for checking SUBACK ����
		p[0] = (packet_id_counter >> 8) & 0xFF;
		p[1] = packet_id_counter & 0xFF;
		p += 2;

		// increment the packet id
		packet_id_counter++;

		p = stringprint(p, topic);

		p[0] = qos;
		p++;

		len = p - packet;
		packet[1] = len - 2; // don't include the 2 bytes of fixed header data
		Logger::warning(this->logger_module_name, F("MQTT subscription packet:"));
		Logger::warning(this->logger_module_name, buffer, len);
		return (len);
	}

	uint8_t MQTT::unsubscribePacket(uint8_t *packet, const char *topic)
	{
		uint8_t *p = packet;
		uint16_t len;

		p[0] = MQTT_CTRL_UNSUBSCRIBE << 4 | 0x1;
		// fill in packet[1] last
		p += 2;

		// packet identifier. used for checking UNSUBACK ������
		p[0] = (packet_id_counter >> 8) & 0xFF;
		p[1] = packet_id_counter & 0xFF;
		p += 2;

		// increment the packet id
		packet_id_counter++;

		p = stringprint(p, topic);

		len = p - packet;
		packet[1] = len - 2; // don't include the 2 bytes of fixed header data
		Logger::warning(this->logger_module_name, F("MQTT unsubscription packet:"));
		Logger::warning(this->logger_module_name, buffer, len);
		return (len);
	}

	uint8_t MQTT::pingPacket(uint8_t *packet)
	{
		packet[0] = MQTT_CTRL_PINGREQ << 4;
		packet[1] = 0;
		Logger::warning(this->logger_module_name, F("MQTT ping packet:"));
		Logger::warning(this->logger_module_name, buffer, 2);
		return (2);
	}

	uint8_t MQTT::pubackPacket(uint8_t *packet, uint16_t packetid)
	{
		packet[0] = MQTT_CTRL_PUBACK << 4;
		packet[1] = 2;
		packet[2] = packetid >> 8;
		packet[3] = packetid;
		Logger::warning(this->logger_module_name, F("MQTT puback packet:"));
		Logger::warning(this->logger_module_name, buffer, 4);
		return (4);
	}

	uint16_t MQTT::readFullPacket(uint8_t *buffer, uint16_t maxsize,
								  uint32_t timeout)
	{
		// will read a packet and Do The Right Thing with length
		uint8_t *pbuff = buffer;

		uint8_t rlen;

		// read the packet type:
		rlen = readPacket(pbuff, 1, timeout);
		if (rlen != 1)
			return (0);

		Logger::warning(this->logger_module_name, F("Packet Type:\t"));
		Logger::warning(this->logger_module_name, pbuff, rlen);
		pbuff++;

		uint16_t value = 0;
		uint32_t multiplier = 1;
		uint8_t encodedByte;

		do
		{
			rlen = readPacket(pbuff, 1, timeout);
			if (rlen != 1)
				return (0);
			encodedByte = pbuff[0]; // save the last read val
			pbuff++;				// get ready for reading the next byte
			uint32_t intermediate = encodedByte & 0x7F;
			intermediate *= multiplier;
			value += intermediate;
			multiplier *= 128;
			if (multiplier > (128UL * 128UL * 128UL))
			{
				Logger::warning(this->logger_module_name,
								F("Malformed packet len\n"));
				return (0);
			}
		} while (encodedByte & 0x80);

		Logger::warning(this->logger_module_name, F("Packet Length:\t"));
		Logger::warning(this->logger_module_name, value);

		if (value > (maxsize - (pbuff - buffer) - 1))
		{
			Logger::warning(this->logger_module_name,
							F("Packet too big for buffer"));
			rlen = readPacket(pbuff, (maxsize - (pbuff - buffer) - 1), timeout);
		}
		else
		{
			rlen = readPacket(pbuff, value, timeout);
		}
		Logger::warning(this->logger_module_name, F("Remaining packet:\t"));
		Logger::warning(this->logger_module_name, pbuff, rlen);

		return ((pbuff - buffer) + rlen);
	}

	uint16_t MQTT::processPacketsUntil(uint8_t *buffer,
									   uint8_t waitforpackettype, uint32_t timeout)
	{
		uint16_t len;

		while (true)
		{
			len = readFullPacket(buffer, MAXBUFFERSIZE, timeout);

			if (len == 0)
			{
				break;
			}

			if ((buffer[0] >> 4) == waitforpackettype)
			{
				return (len);
			}
			else
			{
				Logger::error(this->logger_module_name, F("Dropped a packet"));
			}
		}
		return (0);
	}

	MQTT::MQTT(const char *server, uint16_t port, const char *cid,
			   const char *user, const char *pass, const char *logger_module) : servername(server), portnum(port), clientid(cid), username(user), password(pass), will_topic(0), will_payload(0), will_qos(0), will_retain(0), packet_id_counter(0), logger_module_name(logger_module)
	{
		// reset subscriptions
		for (uint8_t i = 0; i < MAXSUBSCRIPTIONS; i++)
		{
			subscriptions[i] = 0;
		}
	}

	MQTT::MQTT(const char *server, uint16_t port, const char *user,
			   const char *pass, const char *logger_module) : servername(server), portnum(port), clientid(""), username(user), password(pass), will_topic(0), will_payload(0), will_qos(0), will_retain(0), packet_id_counter(0), logger_module_name(logger_module)
	{
		for (uint8_t i = 0; i < MAXSUBSCRIPTIONS; i++)
		{
			subscriptions[i] = 0;
		}
	}

	MQTT::~MQTT()
	{
	}

	int8_t MQTT::connect()
	{
		// Connect to the server.
		if (!connectServer())
			return (-1);

		// Construct and send connect packet.
		uint8_t len = connectPacket(buffer);
		if (!sendPacket(buffer, len))
			return (-1);

		// Read connect response packet and verify it
		len = readFullPacket(buffer, MAXBUFFERSIZE, CONNECT_TIMEOUT_MS);
		if (len != 4)
			return (-1);
		if ((buffer[0] != (MQTT_CTRL_CONNECTACK << 4)) || (buffer[1] != 2))
			return (-1);
		if (buffer[3] != 0)
			return (buffer[3]);

		// Setup subscriptions once connected.
		for (uint8_t i = 0; i < MAXSUBSCRIPTIONS; i++)
		{
			// Ignore subscriptions that aren't defined.
			if (subscriptions[i] == 0)
				continue;

			bool success = false;
			for (uint8_t retry = 0; (retry < 3) && !success; retry++)
			{ // retry until we get a suback
				// Construct and send subscription packet.
				uint8_t len = subscribePacket(buffer, subscriptions[i]->topic,
											  subscriptions[i]->qos);
				if (!sendPacket(buffer, len))
					return (-1);

				if (MQTT_PROTOCOL_LEVEL < 3) // older versions didn't suback
					break;

				// Check for SUBACK if using MQTT 3.1.1 or higher
				// TODO: The Server is permitted to start sending PUBLISH packets matching
				// the Subscription before the Server sends the SUBACK Packet. (will
				// really need to use callbacks - ada)
				Logger::warning(this->logger_module_name, F("looking for suback"));
				if (processPacketsUntil(buffer, MQTT_CTRL_SUBACK,
										SUBACK_TIMEOUT_MS))
				{
					success = true;
					break;
				}
			}
			if (!success)
				return (-2); // failed to sub for some reason
		}

		return (0);
	}

	int8_t MQTT::connect(const char *user, const char *pass)
	{
		username = user;
		password = pass;
		return (connect());
	}

	const __FlashStringHelper *MQTT::connectErrorString(int8_t code)
	{
		switch (code)
		{
		case 1:
			return F(
				"The Server does not support the level of the MQTT protocol requested");
		case 2:
			return F(
				"The Client identifier is correct UTF-8 but not allowed by the Server");
		case 3:
			return F("The MQTT service is unavailable");
		case 4:
			return F("The data in the user name or password is malformed");
		case 5:
			return F("Not authorized to connect");
		case 6:
			return F("Exceeded reconnect rate limit. Please try again later.");
		case 7:
			return F(
				"You have been banned from connecting. Please contact the MQTT "
				"server administrator for more details.");
		case -1:
			return F("Connection failed");
		case -2:
			return F("Failed to subscribe");
		default:
			return F("Unknown error");
		}
	}

	bool MQTT::disconnect()
	{
		// Construct and send disconnect packet.
		uint8_t len = disconnectPacket(buffer);
		if (!sendPacket(buffer, len))
			Logger::warning(this->logger_module_name,
							F("Unable to send disconnect packet"));

		return (disconnectServer());
	}

	bool MQTT::will(const char *topic, const char *payload, uint8_t qos,
					uint8_t retain)
	{

		if (connected())
		{
			Logger::warning(this->logger_module_name,
							F("Will defined after connect"));
			return (false);
		}

		will_topic = topic;
		will_payload = payload;
		will_qos = qos;
		will_retain = retain;

		return (true);
	}

	bool MQTT::publish(const char *topic, const char *payload,
					   uint8_t qos)
	{
		return (publish(topic,
						reinterpret_cast<uint8_t *>(const_cast<char *>(payload)),
						strlen(payload), qos));
	}

	bool MQTT::publish(const char *topic, uint8_t *payload, uint16_t bLen,
					   uint8_t qos)
	{
		// Construct and send publish packet.
		uint16_t len = publishPacket(buffer, topic, payload, bLen, qos);
		if (!sendPacket(buffer, len))
			return (false);

		// If QOS level is high enough verify the response packet.
		if (qos > 0)
		{
			len = readFullPacket(buffer, MAXBUFFERSIZE, PUBLISH_TIMEOUT_MS);
			Logger::warning(this->logger_module_name, F("Publish QOS1+ reply:"));
			Logger::warning(this->logger_module_name, buffer, len);
			if (len != 4)
				return (false);
			if ((buffer[0] >> 4) != MQTT_CTRL_PUBACK)
				return (false);
			uint16_t packnum = buffer[2];
			packnum <<= 8;
			packnum |= buffer[3];

			// we increment the packet_id_counter right after publishing so inc here too
			// to match
			packnum++;
			if (packnum != packet_id_counter)
				return (false);
		}
		return (true);
	}

	bool MQTT::subscribe(MQTT_Subscriber *sub)
	{
		uint8_t i;
		// see if we are already subscribed
		for (i = 0; i < MAXSUBSCRIPTIONS; i++)
		{
			if (subscriptions[i] == sub)
			{
				Logger::warning(this->logger_module_name, F("Already subscribed"));
				return (true);
			}
		}
		if (i == MAXSUBSCRIPTIONS)
		{ // add to subscriptionlist
			for (i = 0; i < MAXSUBSCRIPTIONS; i++)
			{
				if (subscriptions[i] == 0)
				{
					Logger::warning(this->logger_module_name, F("Added sub "));
					Logger::warning(this->logger_module_name, i);
					subscriptions[i] = sub;
					return (true);
				}
			}
		}

		Logger::warning(this->logger_module_name, F("failed "));
		return (false);
	}

	bool MQTT::unsubscribe(MQTT_Subscriber *sub)
	{
		uint8_t i;

		// see if we are already subscribed
		for (i = 0; i < MAXSUBSCRIPTIONS; i++)
		{

			if (subscriptions[i] == sub)
			{

				Logger::warning(this->logger_module_name,
								F(
									"Found matching subscription and attempting to unsubscribe."));

				// Construct and send unsubscribe packet.
				uint8_t len = unsubscribePacket(buffer, subscriptions[i]->topic);

				// sending unsubscribe failed
				if (!sendPacket(buffer, len))
					return (false);

				// if QoS for this subscription is 1 or 2, we need
				// to wait for the unsuback to confirm unsubscription
				if (subscriptions[i]->qos > 0 && MQTT_PROTOCOL_LEVEL > 3)
				{

					// wait for UNSUBACK
					len = readFullPacket(buffer, MAXBUFFERSIZE, CONNECT_TIMEOUT_MS);
					Logger::warning(this->logger_module_name, F("UNSUBACK:\t"));
					Logger::warning(this->logger_module_name, buffer, len);

					if ((len != 5) || (buffer[0] != (MQTT_CTRL_UNSUBACK << 4)))
					{
						return (false); // failure to unsubscribe
					}
				}

				subscriptions[i] = 0;
				return (true);
			}
		}

		// subscription not found, so we are unsubscribed
		return (true);
	}

	MQTT_Subscriber *MQTT::readSubscription(uint32_t timeout)
	{
		uint16_t i, topiclen, datalen;

		// Check if data is available to read.
		uint16_t len = readFullPacket(buffer, MAXBUFFERSIZE, timeout); // return one full packet
		if (!len)
			return (NULL); // No data available, just quit.
		Logger::warning(this->logger_module_name, F("Packet len: "));
		Logger::warning(this->logger_module_name, len);
		Logger::warning(this->logger_module_name, buffer, len);

		if (len < 3)
			return (NULL);
		if ((buffer[0] & 0xF0) != (MQTT_CTRL_PUBLISH) << 4)
			return (NULL);

		// Parse out length of packet.
		topiclen = buffer[3];
		Logger::warning(this->logger_module_name,
						F("Looking for subscription len "));
		Logger::warning(this->logger_module_name, topiclen);

		// Find subscription associated with this packet.
		for (i = 0; i < MAXSUBSCRIPTIONS; i++)
		{
			if (subscriptions[i])
			{
				// Skip this subscription if its name length isn't the same as the
				// received topic name.
				if (strlen(subscriptions[i]->topic) != topiclen)
					continue;
				// Stop if the subscription topic matches the received topic. Be careful
				// to make comparison case insensitive.
				if (strncasecmp(reinterpret_cast<char *>(buffer) + 4,
								subscriptions[i]->topic, topiclen) == 0)
				{
					Logger::warning(this->logger_module_name, F("Found sub #"));
					Logger::warning(this->logger_module_name, i);
					break;
				}
			}
		}
		if (i == MAXSUBSCRIPTIONS)
			return (NULL); // matching sub not found ???

		uint8_t packet_id_len = 0;
		uint16_t packetid = 0;
		// Check if it is QoS 1, TODO: we dont support QoS 2
		if ((buffer[0] & 0x6) == 0x2)
		{
			packet_id_len = 2;
			packetid = buffer[topiclen + 4];
			packetid <<= 8;
			packetid |= buffer[topiclen + 5];
		}

		// zero out the old data
		memset(subscriptions[i]->lastread, 0, SUBSCRIPTIONDATALEN);

		datalen = len - topiclen - packet_id_len - 4;
		if (datalen > SUBSCRIPTIONDATALEN)
		{
			datalen = SUBSCRIPTIONDATALEN - 1; // cut it off
		}
		// extract out just the data, into the subscription object itself
		memmove(subscriptions[i]->lastread, buffer + 4 + topiclen + packet_id_len,
				datalen);
		subscriptions[i]->datalen = datalen;
		Logger::warning(this->logger_module_name, F("Data len: "));
		Logger::warning(this->logger_module_name, datalen);
		Logger::warning(this->logger_module_name, F("Data: "));
		Logger::warning(this->logger_module_name,
						reinterpret_cast<char *>(subscriptions[i]->lastread));

		if ((MQTT_PROTOCOL_LEVEL > 3) && (buffer[0] & 0x6) == 0x2)
		{
			uint8_t ackpacket[4];

			// Construct and send puback packet.
			uint8_t len = pubackPacket(ackpacket, packetid);
			if (!sendPacket(ackpacket, len))
				Logger::warning(this->logger_module_name, F("Failed"));
		}

		// return the valid matching subscription
		return (subscriptions[i]);
	}

	bool MQTT::processPackets(uint32_t timeout)
	{
		uint32_t elapsed = 0, endtime, starttime = millis();
		bool was_sub = 0;
		Logger::notice(this->logger_module_name,
					   F("processPackets"));
		while (elapsed < timeout)
		{

			Logger::notice(this->logger_module_name,
						   F("while"));
			MQTT_Subscriber *sub = readSubscription(timeout - elapsed);
			Logger::notice(this->logger_module_name,
						   F("read subscribitions"));
			if (sub)
			{
				was_sub = 1;
				if (sub->getCallbackBuffer())
				{
					// huh lets do the callback in buffer mode
					Logger::notice(this->logger_module_name,
								   F("*** calling buffer callback with : "));
					Logger::notice(this->logger_module_name,
								   reinterpret_cast<char *>(sub->lastread));
					sub->callbackBuffer(reinterpret_cast<char *>(sub->lastread),
										sub->datalen);
				}
			}

			Logger::notice(this->logger_module_name,
						   F("called callbacks"));
			// keep track over elapsed time
			endtime = millis();
			if (endtime < starttime)
			{
				starttime = endtime; // looped around!")
			}
			elapsed += (endtime - starttime);
			Logger::notice(this->logger_module_name,
						   F("starting while over"));
			Logger::notice(this->logger_module_name,
						   elapsed);
			Logger::notice(this->logger_module_name,
						   was_sub);
		}
		return (was_sub);
	}

	bool MQTT::ping(uint8_t n)
	{ // flushIncoming(100);

		while (n--)
		{
			// Construct and send ping packet.
			uint8_t len = pingPacket(buffer);
			if (!sendPacket(buffer, len))
				continue;

			// Process ping reply.
			len = processPacketsUntil(buffer, MQTT_CTRL_PINGRESP, PING_TIMEOUT_MS);
			if (buffer[0] == (MQTT_CTRL_PINGRESP << 4))
				return (true);
		}

		return (false);
	}

} /* namespace SEFL */

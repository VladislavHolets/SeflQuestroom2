/*
 * seflmqttmanager.cpp
 *
 *  Created on: May 12, 2021
 *      Author: vladi
 */

#include <Client.h>
#include <utils/logger.h>
#include <mqtt_wrappers/mqttmanager.h>
#include <wiring_time.h>
#include <WString.h>
#include <cstring>

namespace SEFL
{

	MQTT_Manager::MQTT_Manager(Client *client, const char *server,
							   uint16_t port, const char *cid, const char *user, const char *pass, const char *logger_module) : MQTT(server, port, cid, user, pass, logger_module), client(client)
	{
	}

	MQTT_Manager::MQTT_Manager(Client *client, const char *server,
							   uint16_t port, const char *user, const char *pass) : MQTT(server, port, user, pass), client(client)
	{
	}

	MQTT_Manager::~MQTT_Manager()
	{
		// TODO Auto-generated destructor stub
	}

	bool MQTT_Manager::connected()
	{ // Return true if connected, false if not connected.
		return (client->connected());
	}

	bool MQTT_Manager::disconnectServer()
	{
		// Stop connection if connected and return success (stop has no indication of
		// failure).
		if (client->connected())
		{
			client->stop();
		}
		return (true);
	}

	uint16_t MQTT_Manager::readPacket(uint8_t *buffer, uint16_t maxlen,
									  uint32_t timeout)
	{ /* Read data until either the connection is closed, or the idle timeout is
	   * reached. */
		uint16_t len = 0;
		uint32_t initial_timeout = timeout;
		uint32_t t = timeout;

		while (client->connected() && (timeout <= initial_timeout))
		{

			while (client->available())
			{
				char c = client->read();
				timeout = t;
				buffer[len] = c;
				len++;

				if (maxlen == 0)
				{ // handle zero-length packets
					return (0);
				}

				if (len == maxlen)
				{ // we read all we want, bail
					SEFL::Logger::notice(this->logger_module_name,
										 F("Read data:\t"));
					SEFL::Logger::notice(this->logger_module_name, buffer, len);
					return (len);
				}
			}
			timeout -= MQTT_CLIENT_READINTERVAL_MS;
			delay(MQTT_CLIENT_READINTERVAL_MS);
		}
		return (len);
	}

	bool MQTT_Manager::sendPacket(uint8_t *buffer, uint16_t len)
	{
		uint16_t ret = 0;

		while (len > 0)
		{
			if (client->connected())
			{
				// send 1000 bytes at most at a time, can adjust this later based on Client

				uint16_t sendlen = len;
				// Serial.print("Sending: "); Serial.println(sendlen);
				// Serial.println(sendlen);
				ret = client->write(buffer, sendlen);
				// Serial.println(ret);
				SEFL::Logger::warning(this->logger_module_name,
									  F("Client sendPacket returned: "));
				SEFL::Logger::warning(this->logger_module_name,
									  ret);
				len -= ret;

				if (ret != sendlen)
				{
					SEFL::Logger::warning(this->logger_module_name,
										  "Failed to send packet.");
					return (false);
				}
			}
			else
			{
				SEFL::Logger::warning(this->logger_module_name,
									  F("Connection failed!"));
				return (false);
			}
		}
		return true;
	}

	bool MQTT_Manager::connectServer()
	{
		// Grab server name from flash and copy to buffer for name resolution.
		memset(buffer, 0, sizeof(buffer));
		strcpy(reinterpret_cast<char *>(buffer), servername);
		SEFL::Logger::warning(this->logger_module_name, F("Connecting to: "));
		SEFL::Logger::warning(this->logger_module_name,
							  reinterpret_cast<char *>(buffer));
		// Connect and check for success (0 result).
		int r = client->connect(reinterpret_cast<char *>(buffer), portnum);
		SEFL::Logger::warning(this->logger_module_name, F("Connect result: "));
		SEFL::Logger::warning(this->logger_module_name, r);
		return (r != 0);
	}

} /* namespace SEFL */

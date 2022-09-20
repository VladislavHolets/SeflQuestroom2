/*
 * seflmqttmanager.h
 *
 *  Created on: May 12, 2021
 *      Author: vladi
 */

#ifndef MQTTMANAGER_H_
#define MQTTMANAGER_H_

#include <mqtt_wrappers/mqtt.h>
#include <sys/_stdint.h>

class Client;

#define MQTT_CLIENT_READINTERVAL_MS 10
namespace SEFL
{

	class MQTT_Manager : public MQTT
	{
	public:
		MQTT_Manager(Client *client, const char *server, uint16_t port,
					 const char *cid, const char *user, const char *pass, const char *logger_module = "mqtt_manager");
		MQTT_Manager(Client *client, const char *server, uint16_t port,
					 const char *user = "", const char *pass = "");

		virtual bool connected() override;

	protected:
		virtual bool disconnectServer() override;
		virtual uint16_t readPacket(uint8_t *buffer, uint16_t maxlen,
									uint32_t timeout) override;
		virtual bool sendPacket(uint8_t *buffer, uint16_t len) override;
		virtual bool connectServer() override;

	public:
		virtual ~MQTT_Manager();

	private:
		Client *client;
	};

} /* namespace SEFL */

#endif /* MQTTMANAGER_H_ */

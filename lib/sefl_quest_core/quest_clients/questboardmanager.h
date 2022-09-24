/*
 * questserver.h
 *
 *  Created on: May 16, 2021
 *      Author: vladi
 */

#ifndef QUESTSERVER_H_
#define QUESTSERVER_H_

#include <constants.h>
#include <mqtt_wrappers/MQTTClientObjectBound.h>
#include <quest_clients/questhostclient.h>
#include <sys/_stdint.h>
#include <Vector.h>

namespace SEFL
{
	class Quest_Board_Manager : public MQTT_Two_Way_Interactor
	{
		struct CallbackItem
		{
			MQTT_Two_Way_Interactor *client;
			String *payload;
		};

	private:
		CallbackItem *callbackQueue_storage[MAX_MESSAGE_QUEUE_AMOUNT];
		Vector<CallbackItem *> callbacksQueue;
		uint32_t callback_timestamp;
		uint32_t shutdown_timestamp;
		const char *name_;
		Language language_;
		bool power_status_;
		Quest_Host_Client host_;

		Quest_Client *clients_storage[MAX_CLIENT_AMOUNT];
		Vector<Quest_Client *> clients_;
		void send_config();
		void setLanguage(Language language_);
		void setPowerStatus(bool power_status_);
		MQTT_Config room_config_;

		bool connect() const;
		bool subscribeAll() const;

	protected:
	public:
		void removeAllClients();
		void removeClient(SEFL::Quest_Client *client);
		Quest_Board_Manager(MQTTClientObjectBound<Quest_Board_Manager> &mqtt, MQTT_Config room_config, const char *host_name,
							const char *placement = DEFAULT_PLACEMENT, const char *server_name = DEFAULT_SERVER_NAME,
							const char *in_topic = DEFAUT_OUT_TOPIC, const char *out_topic = DEFAUT_IN_TOPIC, SEFL::Language language = UKR);
		virtual ~Quest_Board_Manager();
		virtual void inputClb(const char *data, uint16_t len) override;
		void loop();
		bool addClient(SEFL::Quest_Client *client);
		Quest_Host_Client *getHost();
		Language getLanguage() const;
		const char *getName() const;
		bool isPowerStatus() const;
		void checkSubscribitions(String &topic_name, String &payload_val);
		bool pushToCallbacksQueue(MQTT_Two_Way_Interactor &client, String &payload_val);
		void processCallbackQueueOne();
		void processCallbackQueueAll();
		void processCallbackQueue(int number);
	};

} /* namespace SEFL */

#endif /* QUESTSERVER_H_ */

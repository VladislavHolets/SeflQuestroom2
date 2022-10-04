/*
 * sportbike.h
 *
 *  Created on: Jan 31, 2022
 *      Author: piatk
 */

#ifndef SPORTBIKE_H_
#define SPORTBIKE_H_

#include <sefl_quest_core.h>
#include <sys/_stdint.h>

class DFRobotDFPlayerMini;

namespace SEFL
{

	class Sportbike : public Quest_Basic_Client
	{
		uint8_t sportbikeKey, sportbikeLeftButton, sportbikeRightButton, sportbikeThrottle;
		uint8_t sportbikeRideLight, sportbikeRideLightButton, dashboardLight, flagTachometer, flagThrottle, flagFinal;
		uint64_t timing, timerForTrottle, timerForTachometer, timerFinal;
		uint8_t sportbikeLeftRelay, sportbikeRightRelay, sportbikePowerRelay, sportbikeRightRelayFront, sportbikeLeftRelayFront, tachometerPower;

		DFRobotDFPlayerMini *player;

	public:
		Sportbike(MQTTClient &mqtt, const char *name = DEFAULT_DEVICE_NAME,
				  uint8_t reset_status = 1, const char *placement = DEFAULT_PLACEMENT,
				  const char *in_topic = DEFAUT_IN_TOPIC, const char *out_topic = DEFAUT_OUT_TOPIC, SEFL::Language language = UKR);
		virtual ~Sportbike();
		virtual void onActive() override;
		virtual void onFinished() override;
		virtual void onManualFinished() override;
		virtual void onDefault() override;
		DFRobotDFPlayerMini *getPlayer();
		void setPlayer(DFRobotDFPlayerMini *player);
		uint8_t getSportbikeKey();
		void setSportbikeKey(uint8_t sportbikeKey);
		uint8_t getSportbikeLeftButton();
		void setSportbikeLeftButton(uint8_t sportbikeLeftButton);
		uint8_t getSportbikeRideLight();
		void setSportbikeRideLight(uint8_t sportbikeRideLight);
		uint8_t getSportbikeRightButton();
		void setSportbikeRightButton(uint8_t sportbikeRightButton);

		uint8_t getSportbikeThrottle();
		void setSportbikeThrottle(uint8_t sportbikeThrottle);
		uint8_t getSportbikeLeftRelay();
		void setSportbikeLeftRelay(uint8_t sportbikeLeftRelay);
		uint8_t getSportbikeRightRelay();
		void setSportbikeRightRelay(uint8_t sportbikeRightRelay);

		uint8_t getSportbikeRideLightButton() const
		{
			return sportbikeRideLightButton;
		}

		void setSportbikeRideLightButton(uint8_t sportbikeRideLightButton)
		{
			this->sportbikeRideLightButton = sportbikeRideLightButton;
		}

		uint64_t getTiming() const
		{
			return timing;
		}

		void setTiming(uint64_t timing)
		{
			this->timing = timing;
		}

		uint8_t getSportbikePowerRelay();
		void setSportbikePowerRelay(uint8_t sportbikePowerRelay);
		uint8_t getSportbikeLeftRelayFront();
		void setSportbikeLeftRelayFront(uint8_t sportbikeLeftRelayFront);
		uint8_t getSportbikeRightRelayFront();
		void setSportbikeRightRelayFront(uint8_t sportbikeRightRelayFront);
		uint8_t getDashboardLight();
		void setDashboardLight(uint8_t dashboardLight);
		uint8_t getTachometerPower();
		void setTachometerPower(uint8_t tachometerPower);
		uint64_t getTimerForTrottle();
		void setTimerForTrottle(uint64_t timerForTrottle);
		uint64_t getTimerForTachometer();
		void setTimerForTachometer(uint64_t timerForTachometer);
		uint8_t getFlagTachometer();
		void setFlagTachometer(uint8_t flagTachometer);
		uint8_t getFlagThrottle();
		void setFlagThrottle(uint8_t flagThrottle);
		uint8_t getFlagFinal();
		void setFlagFinal(uint8_t flagFinal);
		uint64_t getTimerFinal();
		void setTimerFinal(uint64_t timerFinal);
	};

} /* namespace SEFL */

#endif /* SPORTBIKE_H_ */

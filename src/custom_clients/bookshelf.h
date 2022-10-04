/*
 * bookshelf.h
 *
 *  Created on: Feb 16, 2022
 *      Author: piatk
 */

#ifndef BOOKSHELF_H_
#define BOOKSHELF_H_

#include <PCA9685.h>
#include <sefl_quest_core.h>
#include <sys/_stdint.h>

namespace SEFL
{

	class Bookshelf : public Quest_Basic_Client
	{
		uint8_t number;
		uint8_t numbers[4];
		uint8_t book_servo_pins_[4];
		uint8_t book_sensor_pins_[4];
		const uint8_t order[4] = {2, 1, 3, 0};
		uint8_t book_magnet_pin_;
		PCA9685_ServoEval books_[4];
		uint8_t book_iterator;

	public:
		Bookshelf(MQTTClient &mqtt, uint8_t servo_pins[],
				  uint8_t sensor_pins[], uint8_t book_magnet_pin, const char *name, uint8_t reset_status,
				  const char *placement, const char *in_topic, const char *out_topic,
				  SEFL::Language language);
		virtual ~Bookshelf();
		virtual void onActive() override;
		virtual void onFinished() override;
		virtual void onManualFinished() override;
		virtual void onDefault() override;
	};

} /* namespace SEFL */

#endif /* BOOKSHELF_H_ */

/*
 * tronledmatrixpuzzle.h
 *
 *  Created on: Jun 8, 2021
 *      Author: vladi
 */

#ifndef SEFL_TRONLEDMATRIXPUZZLE_H_
#define SEFL_TRONLEDMATRIXPUZZLE_H_

#include "sefl_quest_core.h"
#include <sys/_stdint.h>

namespace SEFL
{
	struct MatrixColor
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
	};

	class LEDMatrixPuzzle : public Quest_Basic_Client
	{
		const MatrixColor WHITE = {true, true, true};
		const MatrixColor RED = {true, false, false};
		const MatrixColor GREEN = {false, true, false};
		const MatrixColor BLUE = {false, false, true};
		//	const MatrixColor CYAN = { false, true, true };
		//	const MatrixColor MAGENTA = { true, false, true };
		//	const MatrixColor YELLOW = { true, true, false };
		const MatrixColor BLACK = {false, false, false};
		//	const MatrixColor constant_array[8] = { RED, MAGENTA, BLUE, CYAN, GREEN,
		//			YELLOW, WHITE, BLACK };
		const MatrixColor constant_array[5] = {RED, GREEN, BLUE,
											   WHITE, BLACK};
		uint8_t anodes[9];
		const uint8_t cathodes[3];
		const uint8_t anodes_amount = 9;
		const uint8_t cathodes_amount = 3;
		uint8_t current[9][3];
		uint8_t nextColor(uint8_t color);

	public:
		LEDMatrixPuzzle(MQTTClient &mqtt, const char *name = DEFAULT_DEVICE_NAME,
                        uint8_t reset_status = 1, const char *placement = DEFAULT_PLACEMENT,
                        const char *in_topic = DEFAUT_IN_TOPIC, const char *out_topic = DEFAUT_OUT_TOPIC, SEFL::Language language = UKR);
		virtual ~LEDMatrixPuzzle();

		virtual void onActive() override;
		virtual void onFinished() override;
		virtual void onManualFinished() override;
		virtual void onDefault() override;
		const uint8_t *getAnodes();
		void setAnodes(const uint8_t *anodes_);
		const uint8_t *getCathodes();
		void setCathodes(const uint8_t *cathodes_);
		uint8_t getAnodesAmount();
		uint8_t getCathodesAmount();
		uint8_t *getCurrent();
	};

} /* namespace SEFL */

#endif /* SEFL_TRONLEDMATRIXPUZZLE_H_ */

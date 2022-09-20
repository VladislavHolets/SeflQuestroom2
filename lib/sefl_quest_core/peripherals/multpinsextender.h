/*
 * multpinsextender.h
 *
 *  Created on: May 9, 2021
 *      Author: vladi
 */

#ifndef MULTPINSEXTENDER_H_
#define MULTPINSEXTENDER_H_

#include <sys/_stdint.h>
#include <wiring_constants.h>

namespace SEFL {

class Mult_Pins_Extender {
	const uint8_t ci;
	const uint8_t s[4];
	uint8_t last_commute;
	uint8_t commutate(uint8_t pin);
	bool checkDigitalInput(uint8_t pin, uint8_t expected=HIGH);
public:
	Mult_Pins_Extender(uint8_t s0, uint8_t s1, uint8_t s2, uint8_t s3,
			uint8_t ci);
	virtual ~Mult_Pins_Extender();
	void digitalWrite(uint8_t pin, uint8_t val);
	bool digitalRead(uint8_t pin, int16_t trashhold = -1);
	void analogWrite(uint8_t pin, uint16_t val);
	int16_t analogRead(uint8_t pin);
	 uint8_t getCi() ;
	uint8_t getLastCommute() const;
};

} /* namespace SEFL */
extern SEFL::Mult_Pins_Extender Mext;
#endif /* MULTPINSEXTENDER_H_ */

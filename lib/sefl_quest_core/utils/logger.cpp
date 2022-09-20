/*
 * logger.cpp
 *
 *  Created on: May 5, 2021
 *      Author: vladi
 */

#include <sefl/inc/logger.h>
#include <cstring>

#if defined(ARDUINO_ARCH_AVR)
  #include <avr/pgmspace.h>
#endif

// There appears to be an incompatibility with ESP8266 2.3.0.
#if defined(ESP8266)
  #define MEM_TYPE
#else
#define MEM_TYPE PROGMEM
#endif



const char LEVEL_VERBOSE[] MEM_TYPE = "VERBOSE";
const char LEVEL_NOTICE[] MEM_TYPE = "NOTICE";
const char LEVEL_WARNING[] MEM_TYPE = "WARNING";
const char LEVEL_ERROR[] MEM_TYPE = "ERROR";
const char LEVEL_FATAL[] MEM_TYPE = "FATAL";
const char LEVEL_SILENT[] MEM_TYPE = "SILENT";

const char *const LOG_LEVEL_STRINGS[] MEM_TYPE = { LEVEL_VERBOSE, LEVEL_NOTICE,
		LEVEL_WARNING, LEVEL_ERROR, LEVEL_FATAL, LEVEL_SILENT };

void SEFL::Logger::setLogLevel(Level level) {
	Logger::getInstance()->_level = level;
}

SEFL::Logger::Level SEFL::Logger::getLogLevel() {

	return (getInstance()->_level);
}

void SEFL::Logger::verbose(const __FlashStringHelper* message) {
	log(VERBOSE, message);
}

void SEFL::Logger::verbose(const char *module, const __FlashStringHelper* message) {
	log(VERBOSE, module, message);
}

void SEFL::Logger::verbose(const String &message) {

	log(VERBOSE, message);
}

void SEFL::Logger::verbose(const char *module, const String &message) {

	log(VERBOSE, module, message);
}

void SEFL::Logger::verbose(const char *message) {

	log(VERBOSE, message);
}

void SEFL::Logger::verbose(const char *module, const char *message) {

	log(VERBOSE, module, message);
}

void SEFL::Logger::verbose(char message) {

	log(VERBOSE, message);
}

void SEFL::Logger::verbose(const char *module, char message) {

	log(VERBOSE, module, message);
}

void SEFL::Logger::verbose(unsigned char message, int base) {

	log(VERBOSE, message, base);
}

void SEFL::Logger::verbose(const char *module, unsigned char message, int base) {

	log(VERBOSE, module, message, base);
}

void SEFL::Logger::verbose(int message, int base) {

	log(VERBOSE, message, base);
}

void SEFL::Logger::verbose(const char *module, int message, int base) {

	log(VERBOSE, module, message, base);
}

void SEFL::Logger::verbose(unsigned int message, int base) {

	log(VERBOSE, message, base);
}

void SEFL::Logger::verbose(const char *module, unsigned int message, int base) {

	log(VERBOSE, module, message, base);
}

void SEFL::Logger::verbose(long message, int base) {

	log(VERBOSE, message, base);
}

void SEFL::Logger::verbose(const char *module, long message, int base) {

	log(VERBOSE, module, message, base);
}

void SEFL::Logger::verbose(unsigned long message, int base) {

	log(VERBOSE, message, base);
}

void SEFL::Logger::verbose(const char *module, unsigned long message, int base) {

	log(VERBOSE, module, message, base);
}

void SEFL::Logger::verbose(double message, int digits) {

	log(VERBOSE, message, digits);
}

void SEFL::Logger::verbose(const char *module, double message, int digits) {

	log(VERBOSE, module, message, digits);
}

void SEFL::Logger::verbose(const Printable &message) {

	log(VERBOSE, message);
}

void SEFL::Logger::verbose(const char *module, const Printable &message) {

	log(VERBOSE, module, message);
}

void SEFL::Logger::verbose(unsigned char *message, int length) {

	log(VERBOSE, message, length);
}

void SEFL::Logger::verbose(const char *module, unsigned char *message, int length) {

	log(VERBOSE, module, message, length);
}

void SEFL::Logger::notice(const __FlashStringHelper* message) {
	log(NOTICE, message);
}

void SEFL::Logger::notice(const char *module, const __FlashStringHelper* message) {
	log(NOTICE, module, message);
}

void SEFL::Logger::notice(const String &message) {

	log(NOTICE, message);
}

void SEFL::Logger::notice(const char *module, const String &message) {

	log(NOTICE, module, message);
}

void SEFL::Logger::notice(const char *message) {

	log(NOTICE, message);
}

void SEFL::Logger::notice(const char *module, const char *message) {

	log(NOTICE, module, message);
}

void SEFL::Logger::notice(char message) {

	log(NOTICE, message);
}

void SEFL::Logger::notice(const char *module, char message) {

	log(NOTICE, module, message);
}

void SEFL::Logger::notice(unsigned char message, int base) {

	log(NOTICE, message, base);
}

void SEFL::Logger::notice(const char *module, unsigned char message, int base) {

	log(NOTICE, module, message, base);
}

void SEFL::Logger::notice(int message, int base) {

	log(NOTICE, message, base);
}

void SEFL::Logger::notice(const char *module, int message, int base) {

	log(NOTICE, module, message, base);
}

void SEFL::Logger::notice(unsigned int message, int base) {

	log(NOTICE, message, base);
}

void SEFL::Logger::notice(const char *module, unsigned int message, int base) {

	log(NOTICE, module, message, base);
}

void SEFL::Logger::notice(long message, int base) {

	log(NOTICE, message, base);
}

void SEFL::Logger::notice(const char *module, long message, int base) {

	log(NOTICE, module, message, base);
}

void SEFL::Logger::notice(unsigned long message, int base) {

	log(NOTICE, message, base);
}

void SEFL::Logger::notice(const char *module, unsigned long message, int base) {

	log(NOTICE, module, message, base);
}

void SEFL::Logger::notice(double message, int digits) {

	log(NOTICE, message, digits);
}

void SEFL::Logger::notice(const char *module, double message, int digits) {

	log(NOTICE, module, message, digits);
}

void SEFL::Logger::notice(const Printable &message) {

	log(NOTICE, message);
}

void SEFL::Logger::notice(const char *module, const Printable &message) {

	log(NOTICE, module, message);
}

void SEFL::Logger::notice(unsigned char *message, int length) {

	log(NOTICE, message, length);
}

void SEFL::Logger::notice(const char *module, unsigned char *message, int length) {

	log(NOTICE, module, message, length);
}

void SEFL::Logger::warning(const __FlashStringHelper* message) {
	log(WARNING, message);
}

void SEFL::Logger::warning(const char *module, const __FlashStringHelper* message) {
	log(WARNING, module, message);
}

void SEFL::Logger::warning(const String &message) {

	log(WARNING, message);
}

void SEFL::Logger::warning(const char *module, const String &message) {

	log(WARNING, module, message);
}

void SEFL::Logger::warning(const char *message) {

	log(WARNING, message);
}

void SEFL::Logger::warning(const char *module, const char *message) {

	log(WARNING, module, message);
}

void SEFL::Logger::warning(char message) {

	log(WARNING, message);
}

void SEFL::Logger::warning(const char *module, char message) {

	log(WARNING, module, message);
}

void SEFL::Logger::warning(unsigned char message, int base) {

	log(WARNING, message, base);
}

void SEFL::Logger::warning(const char *module, unsigned char message, int base) {

	log(WARNING, module, message, base);
}

void SEFL::Logger::warning(int message, int base) {

	log(WARNING, message, base);
}

void SEFL::Logger::warning(const char *module, int message, int base) {

	log(WARNING, module, message, base);
}

void SEFL::Logger::warning(unsigned int message, int base) {

	log(WARNING, message, base);
}

void SEFL::Logger::warning(const char *module, unsigned int message, int base) {

	log(WARNING, module, message, base);
}

void SEFL::Logger::warning(long message, int base) {

	log(WARNING, message, base);
}

void SEFL::Logger::warning(const char *module, long message, int base) {

	log(WARNING, module, message, base);
}

void SEFL::Logger::warning(unsigned long message, int base) {

	log(WARNING, message, base);
}

void SEFL::Logger::warning(const char *module, unsigned long message, int base) {

	log(WARNING, module, message, base);
}

void SEFL::Logger::warning(double message, int digits) {

	log(WARNING, message, digits);
}

void SEFL::Logger::warning(const char *module, double message, int digits) {

	log(WARNING, module, message, digits);
}

void SEFL::Logger::warning(const Printable &message) {

	log(WARNING, message);
}

void SEFL::Logger::warning(const char *module, const Printable &message) {

	log(WARNING, module, message);
}

void SEFL::Logger::warning(unsigned char *message, int length) {

	log(WARNING, message, length);
}

void SEFL::Logger::warning(const char *module, unsigned char *message, int length) {

	log(WARNING, module, message, length);
}

void SEFL::Logger::error(const __FlashStringHelper* message) {
	log(ERROR, message);
}

void SEFL::Logger::error(const char *module, const __FlashStringHelper* message) {
	log(ERROR, module, message);
}

void SEFL::Logger::error(const String &message) {

	log(ERROR, message);
}

void SEFL::Logger::error(const char *module, const String &message) {

	log(ERROR, module, message);
}

void SEFL::Logger::error(const char *message) {

	log(ERROR, message);
}

void SEFL::Logger::error(const char *module, const char *message) {

	log(ERROR, module, message);
}

void SEFL::Logger::error(char message) {

	log(ERROR, message);
}

void SEFL::Logger::error(const char *module, char message) {

	log(ERROR, module, message);
}

void SEFL::Logger::error(unsigned char message, int base) {

	log(ERROR, message, base);
}

void SEFL::Logger::error(const char *module, unsigned char message, int base) {

	log(ERROR, module, message, base);
}

void SEFL::Logger::error(int message, int base) {

	log(ERROR, message, base);
}

void SEFL::Logger::error(const char *module, int message, int base) {

	log(ERROR, module, message, base);
}

void SEFL::Logger::error(unsigned int message, int base) {

	log(ERROR, message, base);
}

void SEFL::Logger::error(const char *module, unsigned int message, int base) {

	log(ERROR, module, message, base);
}

void SEFL::Logger::error(long message, int base) {

	log(ERROR, message, base);
}

void SEFL::Logger::error(const char *module, long message, int base) {

	log(ERROR, module, message, base);
}

void SEFL::Logger::error(unsigned long message, int base) {

	log(ERROR, message, base);
}

void SEFL::Logger::error(const char *module, unsigned long message, int base) {

	log(ERROR, module, message, base);
}

void SEFL::Logger::error(double message, int digits) {

	log(ERROR, message, digits);
}

void SEFL::Logger::error(const char *module, double message, int digits) {

	log(ERROR, module, message, digits);
}

void SEFL::Logger::error(const Printable &message) {

	log(ERROR, message);
}

void SEFL::Logger::error(const char *module, const Printable &message) {

	log(ERROR, module, message);
}

void SEFL::Logger::error(unsigned char *message, int length) {

	log(ERROR, message, length);
}

void SEFL::Logger::error(const char *module, unsigned char *message, int length) {

	log(ERROR, module, message, length);
}

void SEFL::Logger::fatal(const __FlashStringHelper* message) {
	log(FATAL, message);
}

void SEFL::Logger::fatal(const char *module, const __FlashStringHelper* message) {
	log(FATAL, module, message);
}

void SEFL::Logger::fatal(const String &message) {

	log(FATAL, message);
}

void SEFL::Logger::fatal(const char *module, const String &message) {

	log(FATAL, module, message);
}

void SEFL::Logger::fatal(const char *message) {

	log(FATAL, message);
}

void SEFL::Logger::fatal(const char *module, const char *message) {

	log(FATAL, module, message);
}

void SEFL::Logger::fatal(char message) {

	log(FATAL, message);
}

void SEFL::Logger::fatal(const char *module, char message) {

	log(FATAL, module, message);
}

void SEFL::Logger::fatal(unsigned char message, int base) {

	log(FATAL, message, base);
}

void SEFL::Logger::fatal(const char *module, unsigned char message, int base) {

	log(FATAL, module, message, base);
}

void SEFL::Logger::fatal(int message, int base) {

	log(FATAL, message, base);
}

void SEFL::Logger::fatal(const char *module, int message, int base) {

	log(FATAL, module, message, base);
}

void SEFL::Logger::fatal(unsigned int message, int base) {

	log(FATAL, message, base);
}

void SEFL::Logger::fatal(const char *module, unsigned int message, int base) {

	log(FATAL, module, message, base);
}

void SEFL::Logger::fatal(long message, int base) {

	log(FATAL, message, base);
}

void SEFL::Logger::fatal(const char *module, long message, int base) {

	log(FATAL, module, message, base);
}

void SEFL::Logger::fatal(unsigned long message, int base) {

	log(FATAL, message, base);
}

void SEFL::Logger::fatal(const char *module, unsigned long message, int base) {

	log(FATAL, module, message, base);
}

void SEFL::Logger::fatal(double message, int digits) {

	log(FATAL, message, digits);
}

void SEFL::Logger::fatal(const char *module, double message, int digits) {

	log(FATAL, module, message, digits);
}

void SEFL::Logger::fatal(const Printable &message) {

	log(FATAL, message);
}

void SEFL::Logger::fatal(const char *module, const Printable &message) {

	log(FATAL, module, message);
}

void SEFL::Logger::fatal(unsigned char *message, int length) {

	log(FATAL, message, length);
}

void SEFL::Logger::fatal(const char *module, unsigned char *message, int length) {

	log(FATAL, module, message, length);
}

void SEFL::Logger::log(Level level, const __FlashStringHelper* message) {
	log(level, "", message);
}

void SEFL::Logger::log(Level level, const char *module,
		const __FlashStringHelper* message) {
	if (level <= Logger::getInstance()->getLogLevel()) {
		getInstance()->templateLog(level, module, message);
	}
}

void SEFL::Logger::log(Level level, const String &message) {

	log(level, "", message);
}

void SEFL::Logger::log(Level level, const char *module, const String &message) {
	if (level <= Logger::getInstance()->getLogLevel()) {
		getInstance()->templateLog(level, module, message);
	}
}

void SEFL::Logger::log(Level level, const char *message) {
	log(level, "", message);
}

void SEFL::Logger::log(Level level, const char *module, const char *message) {
	if (level <= Logger::getInstance()->getLogLevel()) {
		getInstance()->templateLog(level, module, message);
	}
}

void SEFL::Logger::log(Level level, char message) {
	log(level, "", message);
}

void SEFL::Logger::log(Level level, const char *module, char message) {
	if (level <= Logger::getInstance()->getLogLevel()) {
		getInstance()->templateLog(level, module, message);
	}
}

void SEFL::Logger::log(Level level, unsigned char message, int base) {
	log(level, "", message, base);
}

void SEFL::Logger::log(Level level, const char *module, unsigned char message,
		int base) {
	if (level <= Logger::getInstance()->getLogLevel()) {
		getInstance()->templateLog(level, module, message, base);
	}
}

void SEFL::Logger::log(Level level, int message, int base) {
	log(level, "", message, base);
}

void SEFL::Logger::log(Level level, const char *module, int message, int base) {
	if (level <= Logger::getInstance()->getLogLevel()) {
		getInstance()->templateLog(level, module, message, base);
	}
}

void SEFL::Logger::log(Level level, unsigned int message, int base) {
	log(level, "", message, base);
}

void SEFL::Logger::log(Level level, const char *module, unsigned int message,
		int base) {
	if (level <= Logger::getInstance()->getLogLevel()) {
		getInstance()->templateLog(level, module, message, base);
	}
}

void SEFL::Logger::log(Level level, long message, int base) {
	log(level, "", message, base);
}

void SEFL::Logger::log(Level level, const char *module, long message, int base) {
	if (level <=Logger::getInstance()->getLogLevel()) {
		getInstance()->templateLog(level, module, message, base);
	}
}

void SEFL::Logger::log(Level level, unsigned long message, int base) {
	log(level, "", message, base);

}

void SEFL::Logger::log(Level level, const char *module, unsigned long message,
		int base) {
	if (level <= Logger::getInstance()->getLogLevel()) {
		getInstance()->templateLog(level, module, message, base);
	}
}

void SEFL::Logger::log(Level level, double message, int digits) {
	log(level, "", message, digits);
}

void SEFL::Logger::log(Level level, const char *module, double message, int digits) {
	if (level <= Logger::getInstance()->getLogLevel()) {
		getInstance()->templateLog(level, module, message, digits);
	}
}

void SEFL::Logger::log(Level level, const Printable &message) {
	log(level, "", message);
}

void SEFL::Logger::log(Level level, const char *module, const Printable &message) {
	if (level <= Logger::getInstance()->getLogLevel()) {
		getInstance()->templateLog(level, module, message);
	}
}

void SEFL::Logger::log(Level level, unsigned char *message, int length) {
	log(level, "", message, length);
}

void SEFL::Logger::log(Level level, const char *module, unsigned char *message,
		int length) {
	if (level <=Logger::getInstance()->getLogLevel()) {
		getInstance()->templateLog(level, module, message, length);
	}
}

void SEFL::Logger::verbose(int64_t message, uint8_t base) {
	log(VERBOSE, "", message, base);
}

void SEFL::Logger::verbose(const char *module, int64_t message, uint8_t base) {
	log(VERBOSE, module, message, base);
}

void SEFL::Logger::verbose(uint64_t message, uint8_t base) {
	log(VERBOSE, "", message, base);
}

void SEFL::Logger::verbose(const char *module, uint64_t message, uint8_t base) {
	log(VERBOSE, module, message, base);
}

void SEFL::Logger::notice(int64_t message, uint8_t base) {
	log(NOTICE, "", message, base);
}

void SEFL::Logger::notice(const char *module, int64_t message, uint8_t base) {
	log(NOTICE, module, message, base);
}

void SEFL::Logger::notice(uint64_t message, uint8_t base) {
	log(NOTICE, "", message, base);
}

void SEFL::Logger::notice(const char *module, uint64_t message, uint8_t base) {
	log(NOTICE, module, message, base);
}

void SEFL::Logger::warning(int64_t message, uint8_t base) {
	log(WARNING, "", message, base);
}

void SEFL::Logger::warning(const char *module, int64_t message, uint8_t base) {
	log(WARNING, module, message, base);
}

void SEFL::Logger::warning(uint64_t message, uint8_t base) {
	log(WARNING, "", message, base);
}

void SEFL::Logger::warning(const char *module, uint64_t message, uint8_t base) {
	log(WARNING, module, message, base);
}

void SEFL::Logger::error(int64_t message, uint8_t base) {
	log(ERROR, "", message, base);
}

void SEFL::Logger::error(const char *module, int64_t message, uint8_t base) {
	log(ERROR, module, message, base);
}

void SEFL::Logger::error(uint64_t message, uint8_t base) {
	log(ERROR, "", message, base);
}

void SEFL::Logger::error(const char *module, uint64_t message, uint8_t base) {
	log(ERROR, module, message, base);
}

void SEFL::Logger::fatal(int64_t message, uint8_t base) {
	log(FATAL, "", message, base);
}

void SEFL::Logger::fatal(const char *module, int64_t message, uint8_t base) {
	log(FATAL, module, message, base);
}

void SEFL::Logger::fatal(uint64_t message, uint8_t base) {
	log(FATAL, "", message, base);
}

void SEFL::Logger::fatal(const char *module, uint64_t message, uint8_t base) {
	log(FATAL, module, message, base);
}

void SEFL::Logger::log(Level level, int64_t message, uint8_t base) {
	log(level, "", message, base);
}

void SEFL::Logger::log(Level level, const char *module, int64_t message,
		uint8_t base) {
	if (level <= Logger::getInstance()->getLogLevel()) {
		getInstance()->templateLog(level, module, message, base);
	}
}

void SEFL::Logger::log(Level level, uint64_t message, uint8_t base) {
	log(level, "", message, base);
}

void SEFL::Logger::log(Level level, const char *module, uint64_t message,
		uint8_t base) {
	if (level <= Logger::getInstance()->getLogLevel()) {
		getInstance()->templateLog(level, module, message, base);
	}
}

SEFL::Logger* SEFL::Logger::getInstance() {
	static SEFL::Logger *logger=new Logger();
	return (logger);
}

const char* SEFL::Logger::asString(Level level) {
	return (LOG_LEVEL_STRINGS[level]);
}

SEFL::Logger::Logger(Print *printer) :
		_level(WARNING), _printer(printer) {
}

void SEFL::Logger::templateLog(Level level, const char *module,
		const __FlashStringHelper* message) {
	if (getInstance()->_printer == nullptr)
		return;
	getInstance()->_printer->print(getInstance()->PRE_LEVEL);
	getInstance()->_printer->print(asString(level));
	getInstance()->_printer->print(getInstance()->POST_LEVEL);

	if (strlen(module) > 0) {
		getInstance()->_printer->print(getInstance()->PRE_MODULE);
		getInstance()->_printer->print(module);
		getInstance()->_printer->print(getInstance()->POST_MODULE);
	}
	getInstance()->_printer->print(getInstance()->PRE_MESSAGE);
	getInstance()->_printer->print(message);
	getInstance()->_printer->print(getInstance()->POST_MESSAGE);
}

void SEFL::Logger::templateLog(Level level, const char *module,
		const String &message) {
	if (getInstance()->_printer == nullptr)
		return;
	getInstance()->_printer->print(getInstance()->PRE_LEVEL);
	getInstance()->_printer->print(asString(level));
	getInstance()->_printer->print(getInstance()->POST_LEVEL);

	if (strlen(module) > 0) {
		getInstance()->_printer->print(getInstance()->PRE_MODULE);
		getInstance()->_printer->print(module);
		getInstance()->_printer->print(getInstance()->POST_MODULE);
	}
	getInstance()->_printer->print(getInstance()->PRE_MESSAGE);
	getInstance()->_printer->print(message);
	getInstance()->_printer->print(getInstance()->POST_MESSAGE);
}

void SEFL::Logger::templateLog(Level level, const char *module, const char *message) {
	if (getInstance()->_printer == nullptr)
		return;
	getInstance()->_printer->print(getInstance()->PRE_LEVEL);
	getInstance()->_printer->print(asString(level));
	getInstance()->_printer->print(getInstance()->POST_LEVEL);

	if (strlen(module) > 0) {
		getInstance()->_printer->print(getInstance()->PRE_MODULE);
		getInstance()->_printer->print(module);
		getInstance()->_printer->print(getInstance()->POST_MODULE);
	}
	getInstance()->_printer->print(getInstance()->PRE_MESSAGE);
	getInstance()->_printer->print(message);
	getInstance()->_printer->print(getInstance()->POST_MESSAGE);
}

void SEFL::Logger::templateLog(Level level, const char *module, char message) {
	if (getInstance()->_printer == nullptr)
		return;
	getInstance()->_printer->print(getInstance()->PRE_LEVEL);
	getInstance()->_printer->print(asString(level));
	getInstance()->_printer->print(getInstance()->POST_LEVEL);

	if (strlen(module) > 0) {
		getInstance()->_printer->print(getInstance()->PRE_MODULE);
		getInstance()->_printer->print(module);
		getInstance()->_printer->print(getInstance()->POST_MODULE);
	}
	getInstance()->_printer->print(getInstance()->PRE_MESSAGE);
	getInstance()->_printer->print(message);
	getInstance()->_printer->print(getInstance()->POST_MESSAGE);
}

void SEFL::Logger::templateLog(Level level, const char *module, unsigned char message,
		int base) {
	if (getInstance()->_printer == nullptr)
		return;
	getInstance()->_printer->print(getInstance()->PRE_LEVEL);
	getInstance()->_printer->print(asString(level));
	getInstance()->_printer->print(getInstance()->POST_LEVEL);

	if (strlen(module) > 0) {
		getInstance()->_printer->print(getInstance()->PRE_MODULE);
		getInstance()->_printer->print(module);
		getInstance()->_printer->print(getInstance()->POST_MODULE);
	}
	getInstance()->_printer->print(getInstance()->PRE_MESSAGE);
	getInstance()->_printer->print(message, base);
	getInstance()->_printer->print(getInstance()->POST_MESSAGE);
}

void SEFL::Logger::templateLog(Level level, const char *module, int message,
		int base) {
	if (getInstance()->_printer == nullptr)
		return;
	getInstance()->_printer->print(getInstance()->PRE_LEVEL);
	getInstance()->_printer->print(asString(level));
	getInstance()->_printer->print(getInstance()->POST_LEVEL);

	if (strlen(module) > 0) {
		getInstance()->_printer->print(getInstance()->PRE_MODULE);
		getInstance()->_printer->print(module);
		getInstance()->_printer->print(getInstance()->POST_MODULE);
	}
	getInstance()->_printer->print(getInstance()->PRE_MESSAGE);
	getInstance()->_printer->print(message, base);
	getInstance()->_printer->print(getInstance()->POST_MESSAGE);
}

void SEFL::Logger::templateLog(Level level, const char *module, unsigned int message,
		int base) {
	if (getInstance()->_printer == nullptr)
		return;
	getInstance()->_printer->print(getInstance()->PRE_LEVEL);
	getInstance()->_printer->print(asString(level));
	getInstance()->_printer->print(getInstance()->POST_LEVEL);

	if (strlen(module) > 0) {
		getInstance()->_printer->print(getInstance()->PRE_MODULE);
		getInstance()->_printer->print(module);
		getInstance()->_printer->print(getInstance()->POST_MODULE);
	}
	getInstance()->_printer->print(getInstance()->PRE_MESSAGE);
	getInstance()->_printer->print(message, base);
	getInstance()->_printer->print(getInstance()->POST_MESSAGE);
}

void SEFL::Logger::templateLog(Level level, const char *module, long message,
		int base) {
	if (getInstance()->_printer == nullptr)
		return;
	getInstance()->_printer->print(getInstance()->PRE_LEVEL);
	getInstance()->_printer->print(asString(level));
	getInstance()->_printer->print(getInstance()->POST_LEVEL);

	if (strlen(module) > 0) {
		getInstance()->_printer->print(getInstance()->PRE_MODULE);
		getInstance()->_printer->print(module);
		getInstance()->_printer->print(getInstance()->POST_MODULE);
	}
	getInstance()->_printer->print(getInstance()->PRE_MESSAGE);
	getInstance()->_printer->print(message, base);
	getInstance()->_printer->print(getInstance()->POST_MESSAGE);
}

void SEFL::Logger::templateLog(Level level, const char *module, unsigned long message,
		int base) {
	if (getInstance()->_printer == nullptr)
		return;
	getInstance()->_printer->print(getInstance()->PRE_LEVEL);
	getInstance()->_printer->print(asString(level));
	getInstance()->_printer->print(getInstance()->POST_LEVEL);

	if (strlen(module) > 0) {
		getInstance()->_printer->print(getInstance()->PRE_MODULE);
		getInstance()->_printer->print(module);
		getInstance()->_printer->print(getInstance()->POST_MODULE);
	}
	getInstance()->_printer->print(getInstance()->PRE_MESSAGE);
	getInstance()->_printer->print(message, base);
	getInstance()->_printer->print(getInstance()->POST_MESSAGE);
}

void SEFL::Logger::templateLog(Level level, const char *module, double message,
		int digits) {
	if (getInstance()->_printer == nullptr)
		return;
	getInstance()->_printer->print(getInstance()->PRE_LEVEL);
	getInstance()->_printer->print(asString(level));
	getInstance()->_printer->print(getInstance()->POST_LEVEL);

	if (strlen(module) > 0) {
		getInstance()->_printer->print(getInstance()->PRE_MODULE);
		getInstance()->_printer->print(module);
		getInstance()->_printer->print(getInstance()->POST_MODULE);
	}
	getInstance()->_printer->print(getInstance()->PRE_MESSAGE);
	getInstance()->_printer->print(message, digits);
	getInstance()->_printer->print(getInstance()->POST_MESSAGE);
}

void SEFL::Logger::templateLog(Level level, const char *module,
		const Printable &message) {
	if (getInstance()->_printer == nullptr)
		return;
	getInstance()->_printer->print(getInstance()->PRE_LEVEL);
	getInstance()->_printer->print(asString(level));
	getInstance()->_printer->print(getInstance()->POST_LEVEL);

	if (strlen(module) > 0) {
		getInstance()->_printer->print(getInstance()->PRE_MODULE);
		getInstance()->_printer->print(module);
		getInstance()->_printer->print(getInstance()->POST_MODULE);
	}
	getInstance()->_printer->print(getInstance()->PRE_MESSAGE);
	getInstance()->_printer->print(message);
	getInstance()->_printer->print(getInstance()->POST_MESSAGE);
}

void SEFL::Logger::templateLog(Level level, const char *module,
		unsigned char *message, int length) {
	if (getInstance()->_printer == nullptr)
		return;
	getInstance()->_printer->print(getInstance()->PRE_LEVEL);
	getInstance()->_printer->print(asString(level));
	getInstance()->_printer->print(getInstance()->POST_LEVEL);

	if (strlen(module) > 0) {
		getInstance()->_printer->print(getInstance()->PRE_MODULE);
		getInstance()->_printer->print(module);
		getInstance()->_printer->print(getInstance()->POST_MODULE);
	}
	getInstance()->_printer->print(getInstance()->PRE_MESSAGE);
	getInstance()->_printer->write(message, length);
	getInstance()->_printer->print(getInstance()->POST_MESSAGE);
}

void SEFL::Logger::templateLog(Level level, const char *module, int64_t message,
		uint8_t base) {
	if (getInstance()->_printer == nullptr)
		return;
	getInstance()->_printer->print(getInstance()->PRE_LEVEL);
	getInstance()->_printer->print(asString(level));
	getInstance()->_printer->print(getInstance()->POST_LEVEL);

	if (strlen(module) > 0) {
		getInstance()->_printer->print(getInstance()->PRE_MODULE);
		getInstance()->_printer->print(module);
		getInstance()->_printer->print(getInstance()->POST_MODULE);
	}
	getInstance()->_printer->print(getInstance()->PRE_MESSAGE);
	getInstance()->_printer->print(message, base);
	getInstance()->_printer->print(getInstance()->POST_MESSAGE);
}

void SEFL::Logger::setLevel(Level level_) {
	getInstance()->_level = level_;
}

 Print* SEFL::Logger::getPrinter()  {
	return (getInstance()->_printer);
}

void SEFL::Logger::setPrinter(Print *printer_) {
	getInstance()->_printer = printer_;
}

void SEFL::Logger::setPostLevel(const char *post_level_) {
	getInstance()->POST_LEVEL = post_level_;
}

void SEFL::Logger::setPostMessage(const char *post_message_) {
	getInstance()->POST_MESSAGE = post_message_;
}

void SEFL::Logger::setPostModule(const char *post_module_) {
	getInstance()->POST_MODULE = post_module_;
}

void SEFL::Logger::setPreLevel(const char *pre_level_ ) {
	getInstance()->PRE_LEVEL = pre_level_;
}

void SEFL::Logger::setPreMessage(const char *pre_message_) {
	getInstance()->PRE_MESSAGE = pre_message_;
}

void SEFL::Logger::setPreModule(const char *pre_module_) {
	getInstance()->PRE_MODULE = pre_module_;
}

SEFL::Logger::Logger() :
				_level(WARNING), _printer(nullptr) {
}

void SEFL::Logger::templateLog(Level level, const char *module, uint64_t message,
		uint8_t base) {
	if (getInstance()->_printer == nullptr)
		return;
	getInstance()->_printer->print(getInstance()->PRE_LEVEL);
	getInstance()->_printer->print(asString(level));
	getInstance()->_printer->print(getInstance()->POST_LEVEL);

	if (strlen(module) > 0) {
		getInstance()->_printer->print(getInstance()->PRE_MODULE);
		getInstance()->_printer->print(module);
		getInstance()->_printer->print(getInstance()->POST_MODULE);
	}
	getInstance()->_printer->print(getInstance()->PRE_MESSAGE);
	getInstance()->_printer->print(message, base);
	getInstance()->_printer->print(getInstance()->PRE_MESSAGE);
}



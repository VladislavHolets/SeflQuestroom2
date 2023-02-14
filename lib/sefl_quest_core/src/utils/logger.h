/*
 * logger.h
 *
 *  Created on: May 5, 2021
 *      Author: vladi
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <Print.h>
#include <sys/_stdint.h>
#include <WString.h>

#if defined(ARDUINO_ARCH_AVR)
  #include <avr/pgmspace.h>
#elif defined(ESP8266)
  #include <pgmspace.h>
#endif

namespace SEFL {

class Logger {
public:
	enum Level {
		NONE=0, VERBOSE, NOTICE, WARNING, ERROR, FATAL, SILENT
	};
	const char *PRE_LEVEL = "[", *POST_LEVEL = "]", *PRE_MODULE = ":",
			*POST_MODULE = " ", *PRE_MESSAGE = " ", *POST_MESSAGE = "\n";

	static void setLogLevel(Level level);

	static Level getLogLevel();

	static void verbose(const __FlashStringHelper *message);
	static void verbose(const char *module, const __FlashStringHelper *message);
	static void verbose(const String &message);
	static void verbose(const char *module, const String &message);
	static void verbose(const char *message);
	static void verbose(const char *module, const char *message);
	static void verbose(char message);
	static void verbose(const char *module, char message);
	static void verbose(unsigned char message, int base = DEC);
	static void verbose(const char *module, unsigned char message, int base =
	DEC);
	static void verbose(int message, int base = DEC);
	static void verbose(const char *module, int message, int base = DEC);
	static void verbose(unsigned int message, int base = DEC);
	static void verbose(const char *module, unsigned int message,
			int base = DEC);
	static void verbose(long message, int base = DEC);
	static void verbose(const char *module, long message, int base = DEC);
	static void verbose(unsigned long message, int base = DEC);
	static void verbose(const char *module, unsigned long message, int base =
	DEC);
	static void verbose(double message, int digits = 2);
	static void verbose(const char *module, double message, int digits = 2);
	static void verbose(const Printable &message);
	static void verbose(const char *module, const Printable &message);
	static void verbose(unsigned char *message, int length = DEC);
	static void verbose(const char *module, unsigned char *message, int length =
	DEC);

	static void notice(const __FlashStringHelper *message);
	static void notice(const char *module, const __FlashStringHelper *message);
	static void notice(const String &message);
	static void notice(const char *module, const String &message);
	static void notice(const char *message);
	static void notice(const char *module, const char *message);
	static void notice(char message);
	static void notice(const char *module, char message);
	static void notice(unsigned char message, int base = DEC);
	static void notice(const char *module, unsigned char message,
			int base = DEC);
	static void notice(int message, int base = DEC);
	static void notice(const char *module, int message, int base = DEC);
	static void notice(unsigned int message, int base = DEC);
	static void notice(const char *module, unsigned int message,
			int base = DEC);
	static void notice(long message, int base = DEC);
	static void notice(const char *module, long message, int base = DEC);
	static void notice(unsigned long message, int base = DEC);
	static void notice(const char *module, unsigned long message,
			int base = DEC);
	static void notice(double message, int digits = 2);
	static void notice(const char *module, double message, int digits = 2);
	static void notice(const Printable &message);
	static void notice(const char *module, const Printable &message);
	static void notice(unsigned char *message, int length = DEC);
	static void notice(const char *module, unsigned char *message, int length =
	DEC);

	static void warning(const __FlashStringHelper *message);
	static void warning(const char *module, const __FlashStringHelper *message);
	static void warning(const String &message);
	static void warning(const char *module, const String &message);
	static void warning(const char *message);
	static void warning(const char *module, const char *message);
	static void warning(char message);
	static void warning(const char *module, char message);
	static void warning(unsigned char message, int base = DEC);
	static void warning(const char *module, unsigned char message, int base =
	DEC);
	static void warning(int message, int base = DEC);
	static void warning(const char *module, int message, int base = DEC);
	static void warning(unsigned int message, int base = DEC);
	static void warning(const char *module, unsigned int message,
			int base = DEC);
	static void warning(long message, int base = DEC);
	static void warning(const char *module, long message, int base = DEC);
	static void warning(unsigned long message, int base = DEC);
	static void warning(const char *module, unsigned long message, int base =
	DEC);
	static void warning(double message, int digits = 2);
	static void warning(const char *module, double message, int digits = 2);
	static void warning(const Printable &message);
	static void warning(const char *module, const Printable &message);
	static void warning(unsigned char *message, int length = DEC);
	static void warning(const char *module, unsigned char *message, int length =
	DEC);

	static void error(const __FlashStringHelper *message);
	static void error(const char *module, const __FlashStringHelper *message);
	static void error(const String &message);
	static void error(const char *module, const String &message);
	static void error(const char *message);
	static void error(const char *module, const char *message);
	static void error(char message);
	static void error(const char *module, char message);
	static void error(unsigned char message, int base = DEC);
	static void error(const char *module, unsigned char message,
			int base = DEC);
	static void error(int message, int base = DEC);
	static void error(const char *module, int message, int base = DEC);
	static void error(unsigned int message, int base = DEC);
	static void error(const char *module, unsigned int message, int base = DEC);
	static void error(long message, int base = DEC);
	static void error(const char *module, long message, int base = DEC);
	static void error(unsigned long message, int base = DEC);
	static void error(const char *module, unsigned long message,
			int base = DEC);
	static void error(double message, int digits = 2);
	static void error(const char *module, double message, int digits = 2);
	static void error(const Printable &message);
	static void error(const char *module, const Printable &message);
	static void error(unsigned char *message, int length = DEC);
	static void error(const char *module, unsigned char *message, int length =
	DEC);

	static void fatal(const __FlashStringHelper *message);
	static void fatal(const char *module, const __FlashStringHelper *message);
	static void fatal(const String &message);
	static void fatal(const char *module, const String &message);
	static void fatal(const char *message);
	static void fatal(const char *module, const char *message);
	static void fatal(char message);
	static void fatal(const char *module, char message);
	static void fatal(unsigned char message, int base = DEC);
	static void fatal(const char *module, unsigned char message,
			int base = DEC);
	static void fatal(int message, int base = DEC);
	static void fatal(const char *module, int message, int base = DEC);
	static void fatal(unsigned int message, int base = DEC);
	static void fatal(const char *module, unsigned int message, int base = DEC);
	static void fatal(long message, int base = DEC);
	static void fatal(const char *module, long message, int base = DEC);
	static void fatal(unsigned long message, int base = DEC);
	static void fatal(const char *module, unsigned long message,
			int base = DEC);
	static void fatal(double message, int digits = 2);
	static void fatal(const char *module, double message, int digits = 2);
	static void fatal(const Printable &message);
	static void fatal(const char *module, const Printable &message);
	static void fatal(unsigned char *message, int length = DEC);
	static void fatal(const char *module, unsigned char *message,
			int length = 0);

	static void log(Level level, const __FlashStringHelper *message);
	static void log(Level level, const char *module,
			const __FlashStringHelper *message);
	static void log(Level level, const String &message);
	static void log(Level level, const char *module, const String &message);
	static void log(Level level, const char *message);
	static void log(Level level, const char *module, const char *message);
	static void log(Level level, char message);
	static void log(Level level, const char *module, char message);
	static void log(Level level, unsigned char message, int base = DEC);
	static void log(Level level, const char *module, unsigned char message,
			int base = DEC);
	static void log(Level level, int message, int base = DEC);
	static void log(Level level, const char *module, int message,
			int base = DEC);
	static void log(Level level, unsigned int message, int base = DEC);
	static void log(Level level, const char *module, unsigned int message,
			int base =
			DEC);
	static void log(Level level, long message, int base = DEC);
	static void log(Level level, const char *module, long message, int base =
	DEC);
	static void log(Level level, unsigned long message, int base = DEC);
	static void log(Level level, const char *module, unsigned long message,
			int base = DEC);
	static void log(Level level, double message, int digits = 2);
	static void log(Level level, const char *module, double message,
			int digits = 2);
	static void log(Level level, const Printable &message);
	static void log(Level level, const char *module, const Printable &message);

	static void log(Level level, unsigned char *message, int length = 0);
	static void log(Level level, const char *module, unsigned char *message,
			int length = 0);

#ifdef SUPPORT_LONGLONG
	static void verbose(int64_t message, uint8_t base = DEC);
	static void verbose(const char *module, int64_t message,
			uint8_t base = DEC);
	static void verbose(uint64_t message, uint8_t base = DEC);
	static void verbose(const char *module, uint64_t message,
			uint8_t base = DEC);

	static void notice(int64_t message, uint8_t base = DEC);
	static void notice(const char *module, int64_t message, uint8_t base = DEC);
	static void notice(uint64_t message, uint8_t base = DEC);
	static void notice(const char *module, uint64_t message,
			uint8_t base = DEC);

	static void warning(int64_t message, uint8_t base = DEC);
	static void warning(const char *module, int64_t message,
			uint8_t base = DEC);
	static void warning(uint64_t message, uint8_t base = DEC);
	static void warning(const char *module, uint64_t message,
			uint8_t base = DEC);

	static void error(int64_t message, uint8_t base = DEC);
	static void error(const char *module, int64_t message, uint8_t base = DEC);
	static void error(uint64_t message, uint8_t base = DEC);
	static void error(const char *module, uint64_t message, uint8_t base = DEC);

	static void fatal(int64_t message, uint8_t base = DEC);
	static void fatal(const char *module, int64_t message, uint8_t base = DEC);
	static void fatal(uint64_t message, uint8_t base = DEC);
	static void fatal(const char *module, uint64_t message, uint8_t base = DEC);

	static void log(Level level, int64_t message, uint8_t base = DEC);
	static void log(Level level, const char *module, int64_t message,
			uint8_t base =
			DEC);
	static void log(Level level, uint64_t message, uint8_t base = DEC);
	static void log(Level level, const char *module, uint64_t message,
			uint8_t base =
			DEC);

#endif

	static Logger* getInstance();

	static const char* asString(Level level);
	void setLevel(Level level_);
	static Print* getPrinter();
	void setPrinter(Print *printer_);
	void setPostLevel(const char *post_level_ = "]");
	void setPostMessage(const char *post_message_ = "\n");
	void setPostModule(const char *post_module_ = " ");
	void setPreLevel(const char *pre_level_ = "[");
	void setPreMessage(const char *pre_message_ = " ");
	void setPreModule(const char *pre_module_ = ":");

private:
	Logger();
	Logger(Print *printer);
	Logger(const Logger&)=delete;
	void operator =(const Logger&)=delete;

	static void templateLog(Level level, const char *module,
			const __FlashStringHelper *message);
	static void templateLog(Level level, const char *module,
			const String &message);
	static void templateLog(Level level, const char *module,
			const char *message);
	static void templateLog(Level level, const char *module, char message);
	static void templateLog(Level level, const char *module,
			unsigned char message, int base = DEC);
	static void templateLog(Level level, const char *module, int message,
			int base = DEC);
	static void templateLog(Level level, const char *module,
			unsigned int message, int base = DEC);
	static void templateLog(Level level, const char *module, long message,
			int base = DEC);
	static void templateLog(Level level, const char *module,
			unsigned long message, int base = DEC);
	static void templateLog(Level level, const char *module, double message,
			int digits = 2);
	static void templateLog(Level level, const char *module,
			const Printable &message);
	static void templateLog(Level level, const char *module,
			unsigned char *message, int length = 0);

#ifdef SUPPORT_LONGLONG
	static void templateLog(Level level, const char *module, int64_t message,
			uint8_t base = DEC);
	static void templateLog(Level level, const char *module, uint64_t message,
			uint8_t base = DEC);
#endif
	Level _level;
	Print *_printer;

};

} /* namespace SEFL */

#endif /* LOGGER_H_ */

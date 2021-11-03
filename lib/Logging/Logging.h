/**
 * Interface for logging different types of messages out to the serial device.
 */

#ifndef LOGGING_H
#define LOGGING_H

#include <Arduino.h>

class Log {
public:
    enum LogLevel {
        PRINT,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        RESPONSE
    };

    static void connect(LogLevel level = RESPONSE, int buad = 9600);

    static void print(const String &message);
    static void debug(const String &message);
    static void info(const String &message);
    static void warn(const String &message);
    static void error(const String &message);
    static void response(const String &message);

private:
    static LogLevel log_level;

    /**
     * Log information into the serial console with the message type and the message.
     * @param type The log message type, like informational or error
     * @param message The message that is reported
     */
    static void log(LogLevel level, const String &message);
};

#endif // LOGGING_H


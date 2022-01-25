#include "Logging.h"

#include <Arduino.h>

Log::LogLevel Log::log_level = NONE;

void Log::connect(LogLevel level, uint32_t baud) {
    log_level = level;
    Serial.begin(baud);
}

void Log::log(LogLevel level, const String &message) {
    if (level == NONE || !Serial || level < log_level) return;

    String qualifier;
    switch (level) {
        case PRINT:
            Serial.println(message);
            return;
        case DEBUG:
            qualifier = "Debug";
            break;
        case INFO:
            qualifier = "Info";
            break;
        case WARN:
            qualifier = "Warn";
            break;
        case ERROR:
            qualifier = "Error";
            break;
        case RESPONSE:
            qualifier = "Response";
            break;
        case NONE:
            return;
    }

    Serial.println(qualifier + ": " + message);
}

void Log::print(const String &message) { log(Log::PRINT, message); }

void Log::debug(const String &message) { log(Log::DEBUG, message); }

void Log::info(const String &message) { log(Log::INFO, message); }

void Log::warn(const String &message) { log(Log::WARN, message); }

void Log::error(const String &message) { log(Log::ERROR, message); }

void Log::response(const String &message) { log(Log::RESPONSE, message); }

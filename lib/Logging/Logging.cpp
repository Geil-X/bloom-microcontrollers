#include "Logging.h"

void log(const String &type, const String &message) {
    Serial.println(type + ":" + message);
}

void debug(const String &message) { log(Log::DEBUG, message); }

void info(const String &message) { log(Log::INFO, message); }

void warn(const String &message) { log(Log::WARN, message); }

void error(const String &message) { log(Log::ERROR, message); }

void response(const String &message) { log(Log::RESPONSE, message); }

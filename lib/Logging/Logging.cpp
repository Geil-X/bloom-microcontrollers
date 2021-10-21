#include "Logging.h"

void Log::log(const String &type, const String &message) {
    if (Serial) Serial.println(type + ":" + message);
}

void Log::debug(const String &message) { log(Log::DEBUG, message); }

void Log::info(const String &message) { log(Log::INFO, message); }

void Log::warn(const String &message) { log(Log::WARN, message); }

void Log::error(const String &message) { log(Log::ERROR, message); }

void Log::response(const String &message) { log(Log::RESPONSE, message); }

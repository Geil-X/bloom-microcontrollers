#include "Logging.h"

void log(const String &type, const String &message) {
    Serial.println(type + ":" + message);
}

void debug(const String &message) { log(DEBUG, message); }

void info(const String &message) { log(INFO, message); }

void warn(const String &message) { log(WARN, message); }

void error(const String &message) { log(ERROR, message); }

void response(const String &message) { log(RESPONSE, message); }

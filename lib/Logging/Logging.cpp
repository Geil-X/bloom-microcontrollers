#include "Logging.h"

void log(const String &type, const String &message) {
    Serial.println(type + ":" + message);
}

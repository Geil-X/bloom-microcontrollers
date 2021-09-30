#include "logging.h"

void log(const String &type, const String &message) {
Serial.println(type + ":" + message);
}

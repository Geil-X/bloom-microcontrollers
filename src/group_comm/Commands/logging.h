#ifndef LOGGING_H
#define LOGGING_H

#include "../../../../.platformio/packages/framework-arduinoteensy/cores/teensy3/Arduino.h"

// Logging Strings
const String DEBUG = "Debug";
const String INFO = "Info";
const String WARN = "Warn";
const String ERROR = "Error";
const String RESPONSE = "Response";

/**
 * Log information into the serial console with the message type and the message.
 * @param type The log message type, like informational or error
 * @param message The message that is reported
 */
void log(const String &type, const String &message);

#endif // LOGGING_H

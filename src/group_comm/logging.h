//
// Created by twate on 6/6/2021.
//

#ifndef FLOWERPOWERIO_LOGGING_H
#define FLOWERPOWERIO_LOGGING_H

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
void log(const String &type, const String &message) {
    Serial.println(type + ":" + message);
}

void invalidArgumentCount(const String &commandName) {
    log(ERROR, "There was an invalid number of arguments provided for \"" + commandName + "\"");
}

void invalidDigitCast(const String &userInput) {
    log(ERROR, "Could not cast user input value \"" + userInput + "\" into an integer.");
}

void invalidCommand(const String &command) {
    log(ERROR, "No command named: \"" + command + "\"");
}

#endif //FLOWERPOWERIO_LOGGING_H

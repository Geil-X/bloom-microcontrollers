/**
 * Arduino/Teensy project for serial communication with a teensy controller.
 */

#include <Arduino.h>

#include "Commands/Command.h"
#include "Commands/CommandFactory.h"

CommandFactory commandFactory = CommandFactory();
Command *command = new NoCommand();

// ---- Program Logic --------------------------------------------------------------------------------------------------

void setup() {
    Serial.begin(9600);
}

void loop() {
    if (Serial.available() == 0) return;
    String serialCommand = Serial.readStringUntil('\n');
    serialCommand.trim();
    serialCommand.toUpperCase();

    // Deallocate previous command and execute the new command
    delete command;
    command = commandFactory.parseCommand(serialCommand);
    command->execute();
}
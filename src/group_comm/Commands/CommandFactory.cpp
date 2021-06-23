#include "CommandFactory.h"

Command *CommandFactory::parseCommand(const String &userInput) {
    processUserCommand(userInput);
    if (userCommandCount == 0) return new NoCommand();

    String commandName = userCommand[0];
    if (commandName == "NEWSTEPPER") return newStepper();
    else if (commandName == "SETUP") return setupStepper();
    else if (commandName == "HOME") return home();
    else if (commandName == "OPEN") return open();
    else if (commandName == "OPENIND") return openInd();
    else if (commandName == "STAT") return stat();
    else if (commandName == "DEBUG") return toggleDebug();
    else if (commandName == "SOFTSTOP") return softStop();
    else if (commandName == "FORCESTOP") return forceStop();
    else if (commandName == "RESET") return reset();
    else if (commandName == "THREHOLD") return threshold();
    else if (commandName == "SPEED") return speed();
    else if (commandName == "ACCELERATION") return acceleration();
    else if (commandName == "PING") return ping();
    else if (commandName == "RESTART") return restart();
    else return new InvalidCommand(commandName);
}

/**
 * Configure a new stepper with the controller
 *
 * Command:
 * NEWSTEPPER <port1> <port2> <sensor_pin>
 */
Command *CommandFactory::newStepper() {
    // Validate command input
    if (userCommandCount != 4) return new InvalidArgumentCount(userCommand[0]);
    else if (!isDecimal(userCommand[1])) return new InvalidDigitCast(userCommand[1]);
    else if (!isDecimal(userCommand[2])) return new InvalidDigitCast(userCommand[2]);
    else if (!isDecimal(userCommand[3])) return new InvalidDigitCast(userCommand[3]);

        // Execute command
    else {
        return new NewStepper(userCommand[1].toInt(), userCommand[2].toInt(), userCommand[3].toInt());
    }
}

/**
 * Run the initial setup Commands for the stepper motors.
 *
 * Command:
 * SETUP
 */
Command *CommandFactory::setupStepper() {
    return new Setup();
}

/**
 * Home some or all of the motors.
 *
 * Command:
 * HOME ALL
 * HOME <motors,*>
 *
 * motors: an index or a comma separated list of indexes without spaces
 */
Command *CommandFactory::home() {
    if (userCommandCount != 2) return new InvalidArgumentCount(userCommand[0]);
    else if (userCommand[1] == "ALL") return new Home(ALL);
    else return new Home(parseNumbers(userCommand[1]));
}

/**
 * Open some or all of the motors to a specified percentage. The open range is between 0 and 100.
 *
 * Command:
 * OPEN <range> ALL
 * OPEN <range> <motors,*>
 *
 * range: Distance to open the motors. Value must be between 0 and 100
 * motors: The motor ids which is an index or comma separated list of indexes
 */
Command *CommandFactory::open() {
    if (userCommandCount != 3) return new InvalidArgumentCount(userCommand[0]);
    else if (!isDecimal(userCommand[1])) return new InvalidDigitCast(userCommand[1]);
    else if (userCommand[2] == "ALL") return new Open(userCommand[1].toInt(), ALL);
    else return new Open(userCommand[1].toInt(), parseNumbers(userCommand[2]));
}

/**
 *  OPENIND <value,*> <value,*>
 */
Command *CommandFactory::openInd() {
    if (userCommandCount != 3) return new InvalidArgumentCount(userCommand[0]);
    return new OpenInd(parseNumbers(userCommand[1]), parseNumbers(userCommand[2]));
}

Command *CommandFactory::stat() {
    return new Stat();
}

Command *CommandFactory::softStop() {
    return new SoftStop();
}

Command *CommandFactory::forceStop() {
    return new ForceStop();
}

Command *CommandFactory::reset() {
    return new Reset();
}

/**
 * Toggle the controller to be in debug mode
 *
 * Command:
 * DEBUG
 */
Command *CommandFactory::toggleDebug() {
    return new ToggleDebug;
}

/**
 * Set the motors threshold values
 *
 * Command:
 * THRESHOLD <id> <stall_threshold> <boundary_offset> <stall_detection_move_block>
 *
 * id: motor id
 * stall_threshold: stall sensitivity
 *     range 0 -> 100
 * boundary_offset: boundary inset from homing extremes
 *     range 0 -> 200
 * stall_detection_move_block:
 *     range 0 -> 100
 */
Command *CommandFactory::threshold() {
    if (userCommandCount != 5) return new InvalidArgumentCount(userCommand[0]);
    else if (!isDecimal(userCommand[1])) return new InvalidDigitCast(userCommand[1]);
    else if (!isDecimal(userCommand[2])) return new InvalidDigitCast(userCommand[2]);
    else if (!isDecimal(userCommand[3])) return new InvalidDigitCast(userCommand[3]);
    else if (!isDecimal(userCommand[4])) return new InvalidDigitCast(userCommand[4]);
    else {
        return new Threshold(
                userCommand[1].toInt(),
                userCommand[2].toInt(),
                userCommand[3].toInt(),
                userCommand[4].toInt());
    }

}

/**
 * Set the speed of the stepper motors
 *
 * Command:
 * SPEED <speed>
 *
 * speed: The speed to set all the steppers in steps per second
 *
 */
Command *CommandFactory::speed() {
    if (userCommandCount != 2) return new InvalidArgumentCount(userCommand[0]);
    else if (!isDecimal(userCommand[1])) return new InvalidDigitCast(userCommand[1]);
    else return new Speed(userCommand[1].toInt());
}

/**
 * Set the speed of the stepper motors
 *
 * Command:
 * ACCELERATION <acceleration>
 *
 * acceleration: The acceleration to set all the steppers in steps per second squared
 */
Command *CommandFactory::acceleration() {
    if (userCommandCount != 2) return new InvalidArgumentCount(userCommand[0]);
    else if (!isDecimal(userCommand[1])) return new InvalidDigitCast(userCommand[1]);
    else return new Acceleration(userCommand[1].toInt());
}

/**
 * Send a ping response back from the microcontroller.
 */
Command *CommandFactory::ping() {
    return new Ping;
}

/**
 * Perform a soft restart on on the microcontroller
 *
 * Command:
 * RESTART
 */
Command *CommandFactory::restart() {
    return new Restart;
}


// ---- Private Functions ----

void CommandFactory::processUserCommand(const String &userInput) {
    Serial.println(userInput);
    int lastIndex = 0;
    userCommandCount = 0;
    int space = userInput.indexOf(' ');

    // Capture all the words with a space following them
    while (space > -1 && userCommandCount < MAX_COMMAND_LENGTH) {
        Serial.println(userInput.substring(lastIndex, space));
        userCommand[userCommandCount] = userInput.substring(lastIndex, space);
        userCommandCount++;

        lastIndex = space + 1;
        space = userInput.indexOf(' ', lastIndex);
    }

    // Make sure to capture the last command in the user input which is not followed by a space
    Serial.println(userInput.substring(lastIndex));
    userCommand[userCommandCount] = userInput.substring(lastIndex);
    userCommandCount++;
}

int *CommandFactory::parseNumbers(String &s) {
    int count = 0;
    for (unsigned int i = 0; i < s.length(); i++)
        if (s[i] == ',') count++;
    int *number = new int[count + 2];
    number[0] = count + 1;
    String rest = s;
    for (int i = 1; i < count + 1; i++) {
        int commaIndex = rest.indexOf(',');
        int value = rest.substring(0, commaIndex).toInt();
        number[i] = value;
        rest = rest.substring(commaIndex + 1);
    }
    int value = rest.toInt();
    number[count + 1] = value;
    return number;
}

bool CommandFactory::isDecimal(const String &value) {
    for (unsigned i = 0; i < value.length(); i++) {
        if (!isDigit(value.charAt(i))) return false;
    }
    return true;
}

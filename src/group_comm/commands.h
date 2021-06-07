#ifndef FLOWERPOWERIO_COMMANDS_H
#define FLOWERPOWERIO_COMMANDS_H

#include <Arduino.h>
#include <Flower.h>
#include <TeensyStep.h>
#include <FlowerGroup.h>

#include "logging.h"

// Flower Defaults
const int STALL_THRESHOLD = 8;
const int BOUNDARY_OFFSET = 40;
const int STALL_DETECTION_MOVE_BLOCK = 40;

// Execution Variables
FlowerGroup group;
bool debug = false;
bool isSetup = false;


// Cpu Restart for Teensy 3.x and other ARM Cortex-M processors
#define CPU_RESTART_ADDR (*(volatile uint32_t *)0xE000ED0C) // Application Interrupt and Reset Control location
#define CPU_RESTART_VAL 0x5FA0004

// Info for parsing and accessing user commands
const int MAX_COMMAND_LENGTH = 10;
String *userCommand = new String[MAX_COMMAND_LENGTH];
int userCommandCount = 0;

// ---- Helper Functions -----------------------------------------------------------------------------------------------

/**
 * Parse a string of comma separated integers into an array of integers.
 * @param s The string to parse
 * @return
 *     The parsed array of integers which can be used with the FlowerGroup functions
 *     The array of integers contains the number of values within it as the first index.
 *     For example, for the values 1,2,3 the array returned from this function is {3, 1, 2, 3}
 */
int *parseNumbers(String s) {
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


/**
 * Process the user input and place the space separated commands into the
 * `userCommands` array. The `userCommandCount` is the accompanying value that
 * tells you how many items are in the `userCommands` array.
 *
 * @param userInput The user input commands
 */
void processUserCommand(const String &userInput) {
    int lastIndex = 0;
    userCommandCount = 0;
    int space = userInput.indexOf(' ');

    // Capture all the words with a space following them
    while (space > -1 && userCommandCount < MAX_COMMAND_LENGTH) {
        userCommand[userCommandCount] = userInput.substring(lastIndex, space);
        userCommandCount++;

        lastIndex = space + 1;
        space = userInput.indexOf(' ', lastIndex);
    }

    // Make sure to capture the last command in the user input which is not followed by a space
    userCommand[userCommandCount] = userInput.substring(lastIndex);
    userCommandCount++;
}

/**
 * Determine if a given string is a decimal or not
 */
bool isDecimal(const String &value) {
    for (unsigned i = 0; i < value.length(); i++) {
        if (!isDigit(value.charAt(i))) return false;
    }
    return true;
}


// ---- Commands -------------------------------------------------------------------------------------------------------

void addStepper(int port1, int port2, int sensorPin) {
    if (group.addStepper(new Flower(port1, port2, sensorPin))) {
        group.stat();
        log(DEBUG, "Flower " + String(port1) + " " + String(port2) + " " + String(sensorPin) + " Created.");
        log(DEBUG, "Flower Added to the group. Total number of Flower:" + String(group.numFlower()));
    } else {
        log(ERROR, "Failed to add flower " + String(port1) + " " + String(port2) + " " + String(sensorPin));
    }
}

/**
 * Configure a new stepper with the controller
 *
 * Command:
 * NEWSTEPPER <port1> <port2> <sensor_pin>
 */
void newStepper(String *commands, int count) {
    // Validate command input
    if (count != 4) invalidArgumentCount(commands[0]);
    else if (!isDecimal(commands[1])) invalidDigitCast(commands[1]);
    else if (!isDecimal(commands[2])) invalidDigitCast(commands[2]);
    else if (!isDecimal(commands[3])) invalidDigitCast(commands[3]);

        // Execute command
    else addStepper(commands[1].toInt(), commands[2].toInt(), commands[3].toInt());
}


/**
 * Run the initial setup commands for the stepper motors.
 *
 * Command:
 * SETUP
 */
void setupStepper() {
    if (!isSetup) {
        log(INFO, "Setup Started");
        group.setup();

        //TODO: Add function for thread
        group.setThreshold(1, STALL_THRESHOLD, BOUNDARY_OFFSET, STALL_DETECTION_MOVE_BLOCK);
        group.setThreshold(2, STALL_THRESHOLD, BOUNDARY_OFFSET, STALL_DETECTION_MOVE_BLOCK);

        isSetup = true;
    }

    log(RESPONSE, "Setup Finished");
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
void home(String *commands, int count) {
    if (count != 2) invalidArgumentCount(commands[0]);
    else if (commands[1] == "ALL") group.setup();
    else group.home(parseNumbers(commands[1]));
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
void open(String *commands, int count) {
    if (count != 3) invalidArgumentCount(commands[0]);
    else if (!isDecimal(commands[1])) invalidDigitCast(commands[1]);
    else if (commands[2] == "ALL") group.move(commands[1].toInt());   // OPEN <range> ALL
    else group.move(commands[1].toInt(), parseNumbers(commands[2]));  // OPEN <range> <motors,*>
}

/**
 *  OPENIND <value,*> <value,*>
 */
void openInd(String *commands, int count) {
    if (count != 3) invalidArgumentCount(commands[0]);
    group.moveInd(parseNumbers(commands[1]), parseNumbers(commands[2]));
}

/**
 * Toggle the controller to be in debug mode
 *
 * Command:
 * DEBUG
 */
void toggleDebug() {
    debug = !debug;
    log(INFO, "Debug Mode :" + String(debug));
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
void threshold(String *commands, int count) {
    if (count != 5) invalidArgumentCount(commands[0]);
    else if (!isDecimal(commands[1])) invalidDigitCast(commands[1]);
    else if (!isDecimal(commands[2])) invalidDigitCast(commands[2]);
    else if (!isDecimal(commands[3])) invalidDigitCast(commands[3]);
    else if (!isDecimal(commands[4])) invalidDigitCast(commands[4]);
    else {
        group.setThreshold(
                commands[1].toInt(),   // id
                commands[2].toInt(),   // stall_threshold
                commands[3].toInt(),   // boundary_offset
                commands[4].toInt());  // stall_detection_move_block
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
void speed(String *commands, int count) {
    if (count != 2) invalidArgumentCount(commands[0]);
    else if (!isDecimal(commands[1])) invalidDigitCast(commands[1]);
    else group.setSpeed(commands[1].toInt());
}

/**
 * Set the speed of the stepper motors
 *
 * Command:
 * ACCELERATION <acceleration>
 *
 * acceleration: The acceleration to set all the steppers in steps per second squared
 */
void acceleration(String *commands, int count) {
    if (count != 2) invalidArgumentCount(commands[0]);
    else if (!isDecimal(commands[1])) invalidDigitCast(commands[1]);
    else group.setAcceleration(commands[1].toInt());
}

/**
 * Send a ping response back from the microcontroller.
 */
void ping() {
    log(RESPONSE, "Ping");
}

/**
 * Perform a soft restart on on the microcontroller
 *
 * Command:
 * RESTART
 */
void softRestart() {
    Serial.end();  // Clear the serial monitor
    CPU_RESTART_ADDR = CPU_RESTART_VAL;  // Set the hardware restart value
}

#endif //FLOWERPOWERIO_COMMANDS_H

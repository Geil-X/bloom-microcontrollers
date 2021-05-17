#include "Flower.h"
#include "TeensyStep.h"
#include "FlowerGroup.h"

const int STALL_THRESHOLD = 8;
const int BOUNDARY_OFFSET = 40;
const int STALL_DETECTION_MOVE_BLOCK = 40;
const String INFO = "Info";
const String WARN = "Warn";
const String ERROR = "Error";
const String RESPONSE = "Response";
FlowerGroup group;
bool debug = false;

void setup() {}

void loop() {
    if (Serial.available() == 0) return;
    String commandValue = Serial.readStringUntil('\n').trim().toUpperCase();
    int spaceIndex = commandValue.indexOf(' ');
    String commandName = getCommandName(commandValue, spaceIndex);

    if (commandName == "NEWSTEPPER") { newStepper(commandValue, spaceIndex); }
    else if (commandName == "SETUP") { setupStepper(); }
    else if (commandName == "HOME") { home(commandValue, spaceIndex); }
    else if (commandName == "OPEN") { open(commandValue, spaceIndex); }
    else if (commandName == "OPENIND") { openInd(commandValue, spaceIndex); }
    else if (commandName == "STAT") { group.stat(); }
    else if (commandName == "DEBUG") { toggleDebug(); }
    else if (commandName == "SOFTSTOP") { group.softStop(); }
    else if (commandName == "FORCESTOP") { group.forceStop(); }
    else if (commandName == "RESET") { group.reset(); }
    else if (commandName == "THREHOLD") { threshold(commandValue, spaceIndex); }
    else if (commandName == "SPEED") { speed(commandValue, spaceIndex); }
    else if (commandName == "ACCELERATION") { acceleration(commandValue, spaceIndex); }
}

// Commands

void newStepper(String commandValue, int spaceIndex) {
    int secondSpaceIndex = commandValue.indexOf(' ', spaceIndex + 1);
    String commandValues = commandValue.substring(spaceIndex + 1);
    int thirdSpaceIndex = commandValue.indexOf(' ', secondSpaceIndex + 1);
    String commandValues2 = commandValue.substring(secondSpaceIndex + 1);
    String commandValues3 = commandValue.substring(thirdSpaceIndex + 1);
    addStepper(commandValues.toInt(), commandValues2.toInt(), commandValues3.toInt());
}

void addStepper(int port1, int port2, int sensorPin) {
    if (group.addStepper(new Flower(port1, port2, sensorPin))) {
        group.stat();
        log(INFO, "Flower " + String(port1) + " " + String(port2) + " " + String(sensorPin) + " Created.");
        log(INFO, "Flower Added to the group. Total number of Flower:" + String(group.numFlower()));
    } else {
        log(ERROR, "Failed to add flower " + String(port1) + " " + String(port2) + " " + String(sensorPin));
    }
}

void setupStepper() {
    log(INFO, "Start Setup");
    group.setup();
    log(INFO, "Finish Setup");
    //TODO: Add function for thread

    group.setThreshold(0, STALL_THRESHOLD, BOUNDARY_OFFSET, STALL_DETECTION_MOVE_BLOCK);
    group.setThreshold(1, STALL_THRESHOLD, BOUNDARY_OFFSET, STALL_DETECTION_MOVE_BLOCK);
}

void home(String commandValue, int spaceIndex) {
    String moveValueString = commandValue.substring(spaceIndex + 1);
    if (moveValueString == "ALL") {
        group.setup();
    } else {
        int moveValue = moveValueString.toInt();
        group.home(parseNumbers(moveValue));
    }
}

void open(String commandValue, int spaceIndex) {
    int secondSpaceIndex = commandValue.indexOf(' ', spaceIndex + 1);
    int moveValue = commandValue.substring(spaceIndex + 1).toInt();
    String commandValues2 = commandValue.substring(secondSpaceIndex + 1);
    if (commandValues2 == "ALL") {
        group.move(moveValue);
    } else {
        group.move(moveValue, parseNumbers(commandValues2));
    }
}

void openInd(String commandValue, int spaceIndex) {
    int secondSpaceIndex = commandValue.indexOf(' ', spaceIndex + 1);
    String moveValue = commandValue.substring(spaceIndex + 1);
    String commandValues2 = commandValue.substring(secondSpaceIndex + 1);
    group.moveInd(parseNumbers(moveValue), parseNumbers(commandValues2));
}

void toggleDebug() {
    debug = !debug;
    log(INFO, "Debug Mode :" + String(debug));
}

void threshold(String commandValue, int spaceIndex) {

    int secondSpaceIndex = commandValue.indexOf(' ', spaceIndex + 1);
    String commandValues = commandValue.substring(spaceIndex + 1);
    int thirdSpaceIndex = commandValue.indexOf(' ', secondSpaceIndex + 1);
    String commandValues2 = commandValue.substring(secondSpaceIndex + 1);
    String commandValues3 = commandValue.substring(thirdSpaceIndex + 1);
    int fourSpaceIndex = commandValue.indexOf(' ', thirdSpaceIndex + 1);
    String commandValues4 = commandValue.substring(fourSpaceIndex + 1);
    log(INFO, commandValues + " " + commandValues2 + " " + commandValues3 + " " + commandValues4);
    group.setThreshold(commandValues.toInt(), commandValues2.toInt(), commandValues3.toInt(),
                      commandValues4.toInt());
}

void speed(String commandValue, int spaceIndex) {
    String speedString = commandValue.substring(spaceIndex + 1);
    group.setSpeed(speedString.toInt());
}

void acceleration(String commandValue, int spaceIndex) {
    String accelerationString = commandValue.substring(spaceIndex + 1);
    group.setAcceleration(accelerationString.toInt());
}


// Helper Functions

String getCommandName(String commandValue, int spaceIndex) {
    if (spaceIndex == -1) {
        return commandValue;
    } else {
        return commandValue.substring(0, spaceIndex);
    }
}

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

void log(String type, String message) {
    Serial.println(type + ":" + message);
}
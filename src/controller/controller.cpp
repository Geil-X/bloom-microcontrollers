#include <Arduino.h>
#include <I2CController.h>

#define NUM_COMMANDS 9
Command *commands[NUM_COMMANDS]{
        new NoCommand(),
        new OpenTo(10),
        new Speed(100),
        new Acceleration(100),
        new Setup(),
        new Home(),
        new Open(),
        new Close(),
        new Ping()};

void setup() {
    Serial.begin(9600);
    Serial.println("Controller:");

    I2CController::join();
}

int current_command = 0;
Command *sent = commands[0];
Command *response = nullptr;

void printCommand(Command *command) {
    if (command == nullptr) {
        Serial.println("Null Pointer");
    }

    switch (command->id) {
        case OPEN_TO: {
            Serial.println("Open To");
            break;
        }
        case SPEED: {
            Serial.println("Speed");
            break;
        }
        case ACCELERATION: {
            Serial.println("Acceleration");
            break;
        }
        case NO_COMMAND: {
            Serial.println("No Command");
            break;
        }
        case SETUP: {
            Serial.println("Setup");
            break;
        }
        case HOME: {
            Serial.println("Home");
            break;
        }
        case OPEN: {
            Serial.println("Open");
            break;
        }
        case CLOSE: {
            Serial.println("Close");
            break;
        }
        case PING: {
            Serial.println("Ping");
            break;
        }
        case INVALID_COMMAND: {
            Serial.println("Invalid Command");
            break;
        }
    }
}

void loop() {
    sent = commands[current_command];

    Serial.print("Sending command: ");
    printCommand(sent);
    I2CController::sendCommand(8, sent);

    delete response;
    Serial.print("Receiving command: ");
    response = I2CController::requestCommand(8);
    printCommand(response);

    current_command = (current_command + 1) % NUM_COMMANDS;
    delay(1000);
}

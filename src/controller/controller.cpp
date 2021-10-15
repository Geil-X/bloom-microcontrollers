#include <Arduino.h>
#include <I2CController.h>

#define NUM_COMMANDS 9
Command *commands[NUM_COMMANDS]{
        new NoCommand(),
        new OpenTo(10),
        new Speed(5000),
        new Acceleration(500),
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

void loop() {
    sent = commands[current_command];

    Serial.println("Sending command:  " + sent->toString());
    I2CController::sendCommand(8, sent);

    delete response;
    response = I2CController::requestCommand(16);
    Serial.println("Receiving command: " + response->toString());

    current_command = (current_command + 1) % NUM_COMMANDS;
    delay(1000);
}

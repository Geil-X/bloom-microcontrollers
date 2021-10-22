#include <Arduino.h>

#include <I2CController.h>
#include <Command.h>
#include <Debug.h>

#define DEVICE_ID 16
#define COMMUNICATION_DELAY 25
#define ANALOG_MAX 1023

#define POTENTIOMETER_PIN A0

Command *home = new Home();
Command *speed = new Speed(MICROSTEPS * 10000);
Command *acceleration = new Acceleration(MICROSTEPS * 5000);
Command *command;

void setup() {
    Serial.begin(9600);

    I2CController::join();

    delay(COMMUNICATION_DELAY);
    I2CController::sendCommand(DEVICE_ID, speed);
    delay(COMMUNICATION_DELAY);
    I2CController::sendCommand(DEVICE_ID, acceleration);
    delay(COMMUNICATION_DELAY);
}

int position() {
    return (int) map(analogRead(POTENTIOMETER_PIN), 0, ANALOG_MAX, 100, 0);
}

void loop() {
    command = new OpenTo(position());
    I2CController::sendCommand(DEVICE_ID, command);
    delay(COMMUNICATION_DELAY);
    Serial.println(freeMemory());
    delete command;
}

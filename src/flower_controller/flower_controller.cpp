#include <Arduino.h>

#include "../../lib/Sensors/Button.h"
#include "../../lib/Sensors/PotentiometerButton.h"
#include <Command.h>
#include <I2CController.h>

#define HOME_PIN 4
#define OPEN_PIN 5
#define CLOSE_PIN 6
#define SPEED_PIN 7
#define ACCELERATION_PIN 8
#define OPEN_TO_PIN 9
#define SPEED_POT_PIN A0
#define ACCELERATION_POT_PIN A1
#define PERCENTAGE_POT_PIN A2

#define ANALOG_IN_MAX 255

#define MAX_SPEED 30000
#define MIN_SPEED 1000

#define MAX_ACCELERATION 10000
#define MIN_ACCELERATION 500

#define DEVICE_ID 16

Button homeButton = Button(HOME_PIN);
Button openButton = Button(OPEN_PIN);
Button closeButton = Button(CLOSE_PIN);
PotentiometerButton speedButton = PotentiometerButton(SPEED_PIN, SPEED_POT_PIN, MIN_SPEED, MAX_SPEED);
PotentiometerButton accelerationButton = PotentiometerButton(ACCELERATION_PIN, ACCELERATION_POT_PIN, MIN_ACCELERATION, MAX_ACCELERATION);
PotentiometerButton openToButton = PotentiometerButton(OPEN_TO_PIN, PERCENTAGE_POT_PIN, 0, 100);

Command *command = nullptr;

void setup() {
    Serial.begin(9600);
    Serial.println("Running...");

    I2CController::join();
}

void checkButtons() {
    if (homeButton.isPressed()) { command = new Home(); }
    if (openButton.isPressed()) { command = new Open(); }
    if (closeButton.isPressed()) { command = new Close(); }
    if (speedButton.isPressed()) { command = new Speed(speedButton.value()); }
    if (accelerationButton.isPressed()) { command = new Acceleration(accelerationButton.value()); }
    if (openToButton.isPressed()) { command = new OpenTo(openToButton.value()); }
}

void loop() {
    checkButtons();

    if (command != nullptr) {
        Serial.println(command->toString());
        I2CController::sendCommand(DEVICE_ID, command);
        delete command;
        command = nullptr;
    }
}
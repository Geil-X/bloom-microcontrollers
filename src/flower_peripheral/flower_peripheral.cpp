#include <Arduino.h>
#include <I2CPeripheral.h>
#include <Debug.h>

// I2C Communication
#define DEVICE_ID 16  // I2C Address

// Stepper Pins
#define DIAG1_PIN   3
#define EN_PIN      7
#define DIR     8
#define STEP    9
#define CS      10

// Stepper Driver SPI Communication
#define MOSI    11  // SDI
#define MISO    12  // SDO
#define SCK     13  // SPI Reference Clock

Flower flower = Flower(EN_PIN, DIR, STEP, CS, MOSI, MISO, SCK, DIAG1_PIN);
Command *command;

void setup() {
    I2CPeripheral::join(DEVICE_ID);

    flower.setup();
    flower.home();
}

void loop() {
    command = I2CPeripheral::tryGetCommand();

    if (command != nullptr) {
        command->execute(flower);
        delete command;
    }

    flower.run();
}
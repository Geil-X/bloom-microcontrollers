#include <Arduino.h>
#include <I2CPeripheral.h>

// I2C Communication
#define DEVICE_ID 17  // I2C Address

// Pin Definitions for different microcontrollers

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

void setup() {
    I2CPeripheral::join(DEVICE_ID);

    flower.setup();
    flower.home();
    flower.setMaxSpeed(10000);
    flower.setAcceleration(5000);
}

void loop() {
    I2CPeripheral::executeCommand(flower);

    if (flower.motorStalled()) {
        delay(500);
        flower.home();
    }

    flower.run();
}
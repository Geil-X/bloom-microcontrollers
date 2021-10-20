#include <Arduino.h>
#include <I2CPeripheral.h>

// I2C Communication
#define DEVICE_ID 16  // I2C Address

// Pin Definitions for different microcontrollers

#if defined(UNO)

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

#elif defined(ATMEGA2560)

// SPI Communication
#define MISO  50  // SDO
#define MOSI  51  // SDI
#define SCK   52  // SPI Reference Clock
#define CS    25

// Stepper Pins
#define STEP      2
#define DIAG1_PIN 3
#define DIR       4
#define EN_PIN    5

#endif

Flower flower = Flower(EN_PIN, DIR, STEP, CS, MOSI, MISO, SCK, DIAG1_PIN);

void setup() {
    I2CPeripheral::join(DEVICE_ID);

    flower.setup();
    flower.home();
}

void loop() {
    I2CPeripheral::executeCommand(flower);
    flower.run();
}
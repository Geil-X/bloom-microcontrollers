#include <Flower.h>

// Stepper Pins
#define DIAG1_PIN   3
#define EN_PIN      7
#define DIR     8
#define STEP    9
#define CS      10

// SPI Communication
#define MOSI    11  // SDI
#define MISO    12  // SDO
#define SCK     13  // SPI Reference Clock

// TMC2130 Parameters
#define R_SENSE 0.11f  // Set for the silent step stick series

Flower flower = Flower(EN_PIN, DIR, STEP, CS, MOSI, MISO, SCK, DIAG1_PIN, R_SENSE);

void setup() {
    Serial.begin(9600);

    flower.setup();
    flower.home();

    flower.setMaxSpeed(1000);
    flower.setAcceleration(250);
}

void loop() {
    delay(500);
    flower.open();
    delay(500);
    flower.close();
}
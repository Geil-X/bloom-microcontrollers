#include <Flower.h>

// Stepper Pins
#define DIAG1   3
#define EN      7
#define DIR     8
#define STEP    9
#define CS      10

// SPI Communication
#define MOSI    11  // SDI
#define MISO    12  // SDO
#define SCK     13  // SPI Reference Clock

// TMC2130 Parameters
#define R_SENSE 0.11f  // Set for the silent step stick series

Flower flower = Flower(EN, DIR, STEP, CS, MOSI, MISO, SCK, DIAG1, R_SENSE);

void setup() {
//    Serial.begin(9600);
    flower.setup();
}

void loop() {
    if (flower.remainingDistance() <= 0) {
        flower.move(40);
        flower.reverse();
    }

    if (Flower::stalled) {
        Serial.println("Motor Stalled");
        Flower::stalled = false;
    }


    flower.run();
}
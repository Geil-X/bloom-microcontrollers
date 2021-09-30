//#include <Flower.h>
#include "../../lib/Flower/Flower.h"

#define SENSOR  6

// Stepper Pins
#define EN      7
#define DIR     8
#define STEP    9
#define CS      10

// SPI Communication
#define MOSI    11 // SDI
#define MISO    12 // SDO
#define SCK     13 // SPI Reference Clock

#define R_SENSE 0.11f  // Set for the silent step stick series

Flower flower = Flower(EN, DIR, STEP, CS, MOSI, MISO, SCK, R_SENSE);

void setup() {
    flower.setup();
}


void loop() {
    flower.run();
}
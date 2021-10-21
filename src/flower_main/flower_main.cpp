#include <Arduino.h>
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

Flower flower = Flower(EN_PIN, DIR, STEP, CS, MOSI, MISO, SCK, DIAG1_PIN);
Direction previous_position = DIRECTION_CLOSE;

void setSpeed() {
    flower.setMaxSpeed(MICROSTEPS * 1000);
    flower.setAcceleration(MICROSTEPS * 200);
}

void setup() {
    Serial.begin(9600);

    flower.setup();
    flower.home();
    setSpeed();
}

void loop() {
    if (flower.remainingDistance() == 0) {
        switch (previous_position) {
            case DIRECTION_OPEN: {
                Serial.println("Close");
                flower.closeAsync();
                previous_position = DIRECTION_CLOSE;
                break;
            }
            case DIRECTION_CLOSE: {
                Serial.println("Open");
                flower.openAsync();
                previous_position = DIRECTION_OPEN;
                break;
            }
        }
    }

    if (flower.motorStalled()) {
        delay(1000);
        flower.home();
        setSpeed();
        flower.closeAsync();
        previous_position = DIRECTION_CLOSE;
    }

    flower.run();
}
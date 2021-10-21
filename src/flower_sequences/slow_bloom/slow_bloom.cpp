#include <Arduino.h>

#include <Command.h>
#include <MathExtra.h>
#include <easing.h>
//#include <sdnoise1234.h>

// Constants
#define MS_PER_SECOND 1000.

// Device Parameters
#define DEVICE_ID 16
#define POLL_TIME 50 // ms

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
#define DEVICE_ID 16  // I2C Address

// Choreography parameters
#define SPEED MICROSTEPS * 1000 // micro steps / s
#define ACCELERATION MICROSTEPS * 500 // micro steps / s

#define BLOOM_PERIOD (float) 5. // s
#define NUMBER_OF_BLOOMS (float) 4
#define FINISH_TIME (float) 1. // s

const float bloom_time = BLOOM_PERIOD * (NUMBER_OF_BLOOMS - 0.5);
const float sequence_time = bloom_time + FINISH_TIME;
const unsigned int sequence_time_ms = (int) (sequence_time * MS_PER_SECOND);

Flower flower = Flower(EN_PIN, DIR, STEP, CS, MOSI, MISO, SCK, DIAG1_PIN);
Command *command;
unsigned long lastCommand = 0;

void setup() {
    flower.setup();
    flower.home();

    flower.setMaxSpeed(SPEED);
    flower.setAcceleration(ACCELERATION);
}

float sequencePosition(float seconds) {
    if (seconds > bloom_time) {
        return 100.;
    } else {
        float ramp = linearRamp(seconds, 0, sequence_time);
        float bloom = ncos(seconds, BLOOM_PERIOD);
        return map(ramp * bloom, 0., 1., 0., 100.);
    }
}

void loop() {
    if (millis() - lastCommand > POLL_TIME) {
        lastCommand = millis();
        auto seconds = float((float) (millis() % sequence_time_ms) / 1000.);
        delete command;
        command = new OpenTo(sequencePosition(seconds));
        command->execute(flower);
    }

    if (flower.motorStalled()) {
        delay(250);
        flower.home();
    }

    flower.run();
}
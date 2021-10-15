#include <Arduino.h>

#include <Command.h>
#include <MathExtra.h>
#include <easing.h>
//#include <sdnoise1234.h>

// Constants
#define MS_PER_SECOND 1000.

// Device Parameters
#define DEVICE_ID 16
#define POLL_TIME 250 // ms

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
#define SEQUENCE_TIME 60. // s
#define BLOOM_PERIOD 10. // s
#define NOISE_PERIOD 1. // s
#define SPEED MICROSTEPS * 1000 // micro steps / s
#define ACCELERATION MICROSTEPS * 500 // micro steps / s
#define FINISH_TIME 1. // s

Flower flower = Flower(EN_PIN, DIR, STEP, CS, MOSI, MISO, SCK, DIAG1_PIN);
Command *command;
unsigned long lastCommand = 0;
const unsigned int sequence_time_ms = (SEQUENCE_TIME + FINISH_TIME) * MS_PER_SECOND;

void setup() {
    flower.setup();
    flower.home();

    flower.setMaxSpeed(SPEED);
    flower.setAcceleration(ACCELERATION);
}

float sequencePosition(float seconds) {
    if (seconds > SEQUENCE_TIME - BLOOM_PERIOD / 2) {
        return 100.;
    } else {
//        auto pnoise = (float) ((1 + sdnoise1(seconds / NOISE_PERIOD, nullptr)) / 2);
        float ramp = linearRamp(seconds, 0, SEQUENCE_TIME);
        float bloom = ncos(seconds, BLOOM_PERIOD);
        return easeOutQuad(ramp * bloom);
    }
}

void loop() {
    if (millis() - lastCommand > POLL_TIME) {
        auto seconds = float((float) (millis() % sequence_time_ms) / 1000.);
        delete command;
        float openToAmount = map(sequencePosition(seconds), 0., 1., 0., 100.);
        command = new OpenTo(openToAmount);
        command->execute(flower);
    }

    flower.run();
}

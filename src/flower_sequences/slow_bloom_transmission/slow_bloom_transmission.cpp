#include <Arduino.h>

#include <Command.h>
#include <MathExtra.h>
#include <I2CController.h>


// Constants
#define MS_PER_SECOND 1000.

// Device Parameters
#define DEVICE_ID_1 16
#define DEVICE_ID_2 17
#define POLL_TIME 50 // ms

// Choreography parameters
#define SPEED MICROSTEPS * 1000 // micro steps / s
#define ACCELERATION MICROSTEPS * 500 // micro steps / s

#define BLOOM_PERIOD (float) 5. // s
#define NUMBER_OF_BLOOMS (float) 4
#define FINISH_TIME (float) 1. // s

const float bloom_time = BLOOM_PERIOD * (NUMBER_OF_BLOOMS - 0.5);
const float sequence_time = bloom_time + FINISH_TIME;
const unsigned int sequence_time_ms = (int) (sequence_time * MS_PER_SECOND);

unsigned long lastCommand = 0;

void setup() {
    I2CController::join();
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
        Packet positionPacket = I2CCommandFactory::openToPacket(sequencePosition(seconds));
        I2CController::sendPacket(DEVICE_ID_1, positionPacket);
        I2CController::sendPacket(DEVICE_ID_2, positionPacket);
    }
}
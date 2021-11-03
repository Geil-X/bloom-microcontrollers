#ifndef FLOWER_NOISE_CHOREOGRAPHY_H
#define FLOWER_NOISE_CHOREOGRAPHY_H

#include <Arduino.h>

#include <I2CController.h>
#include <MathExtra.h>
#include <DistanceSensor.h>
#include <Choreography.h>
#include <noise/noise1234.h>
#include <easing.h>

// Device Parameters
#define COMMAND_DELAY 5 // ms

// Running Variables
#define TRIGGER_PIN 7
#define ECHO_PIN 6
DistanceSensor distance_sensor(TRIGGER_PIN, ECHO_PIN);

#define SEQUENCE_COUNT 1
Choreography<SEQUENCE_COUNT> choreography;

#define DEVICE_COUNT 2
int devices[DEVICE_COUNT] = {16, 17};
unsigned long last_command;

/** Return the relative distance to an object. Distance is between 0 and 1 where 0 is close and 1 is far away. */
float relativeDistance() {
#define MAX_DISTANCE 3
#define MIN_DISTANCE 1
    float distance = constrain(distance_sensor.distanceFt(), MIN_DISTANCE, MAX_DISTANCE);
    return (distance - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE);
}

#define DURATION 600.f

float wander(int device, float t, float x, float y) {
    float seconds_per_cycle = 5.f;
    float cycles = DURATION / seconds_per_cycle;
    float noise = noise3(t * cycles, x, y);
    float normalized_noise = (1 + noise) / 2;
    float easedNoise = easeInOutSine(normalized_noise);

    // Close the flower when someone comes close
    return lerp(relativeDistance(), 0.f, easedNoise);
}

void setup() {
    Serial.begin(9600);
    I2CController::join();
    choreography = Choreography<SEQUENCE_COUNT>()
            .addSequence(DURATION, wander);
}

void sendCommand() {
    for (int device = 0; device < DEVICE_COUNT; device++) {
        float position = choreography.perform(device, millis(), (float) device * 5.27f, (float) device * 6.87f);
        float motorPosition = map(position, 0, 1, 0, 100);
        Packet positionPacket = I2CCommandFactory::openToPacket(motorPosition);

        int device_id = devices[device];
        I2CController::sendPacket(device_id, positionPacket);
    }
}

void loop() {
    unsigned long milliseconds = millis();
    if (millis() - last_command > COMMAND_DELAY) {
        sendCommand();
        last_command = milliseconds;
    }
}

#endif //FLOWER_NOISE_CHOREOGRAPHY_H

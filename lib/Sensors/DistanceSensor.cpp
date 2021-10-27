#include "DistanceSensor.h"
#include <Arduino.h>

// Constants for the speed of sound
#define MICROSECONDS_PER_INCH 74.0524781f
#define MICROSECONDS_PER_FOOT 888.629738f
#define MICROSECONDS_PER_CM 29.154519f

DistanceSensor::DistanceSensor(uint8_t triggerPin, uint8_t echoPin) {
    this->triggerPin = triggerPin;
    this->echoPin = echoPin;

    pinMode(triggerPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

float DistanceSensor::distanceCm() {
    return durationMicroSeconds() / MICROSECONDS_PER_CM;
}

float DistanceSensor::distanceIn() {
    return durationMicroSeconds() / MICROSECONDS_PER_INCH;
}

float DistanceSensor::distanceFt() {
    return durationMicroSeconds() / MICROSECONDS_PER_FOOT;
}

float DistanceSensor::durationMicroSeconds() {
    // Ensure that the trigger pin is low
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);

    // Send out a short burst to detect
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10); // Added this line

    // Cancel the pulse and wait for the reply
    digitalWrite(triggerPin, LOW);
    float duration = (float) pulseIn(echoPin, HIGH) / 2.f;

    runningDuration.add(duration);
    return runningDuration.getMedian();
}


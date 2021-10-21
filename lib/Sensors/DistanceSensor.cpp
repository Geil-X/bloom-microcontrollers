#include "DistanceSensor.h"
#include <Arduino.h>

DistanceSensor::DistanceSensor(uint8_t triggerPin, uint8_t echoPin) {
    this->triggerPin = triggerPin;
    this->echoPin = echoPin;

    pinMode(triggerPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

float DistanceSensor::distanceCm() const {
    float us = durationMicroSeconds();
    return us / MICROSECONDS_PER_CM;
}

float DistanceSensor::distanceIn() const {
    float us = durationMicroSeconds();
    return us / MICROSECONDS_PER_INCH;
}

float DistanceSensor::durationMicroSeconds() const {
    // Ensure that the trigger pin is low
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);

    // Send out a short burst to detect
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10); // Added this line

    // Cancel the pulse and wait for the reply
    digitalWrite(triggerPin, LOW);
    return (float) pulseIn(echoPin, HIGH) / 2.f;
}

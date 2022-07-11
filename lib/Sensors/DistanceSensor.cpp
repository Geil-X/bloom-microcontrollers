#include <DistanceSensor.h>

#include <Arduino.h>


// Constants for the speed of sound
#define MICROSECONDS_PER_INCH 74.0524781f
#define MICROSECONDS_PER_FOOT 888.629738f
#define MICROSECONDS_PER_CM 29.154519f

DistanceSensor::DistanceSensor(uint8_t trigger_pin, uint8_t echo_pin) {
    this->trigger_pin = trigger_pin;
    this->echo_pin = echo_pin;

    pinMode(trigger_pin, OUTPUT);
    pinMode(echo_pin, INPUT);
}

float DistanceSensor::distanceCm(float max) {
    auto timeout_microseconds = (unsigned long) (max * MICROSECONDS_PER_CM) * 2;
    return durationMicroSeconds(timeout_microseconds) / MICROSECONDS_PER_CM;
}

float DistanceSensor::distanceIn(float max) {
    auto timeout_microseconds = (unsigned long) (max * MICROSECONDS_PER_INCH) * 2;
    return durationMicroSeconds(timeout_microseconds) / MICROSECONDS_PER_INCH;
}

float DistanceSensor::distanceFt(float max) {
    auto timeout_microseconds = (unsigned long) (max * MICROSECONDS_PER_FOOT) * 2;
    return durationMicroSeconds(timeout_microseconds) / MICROSECONDS_PER_FOOT;
}

float DistanceSensor::durationMicroSeconds(unsigned long max_ms) {
    // Ensure that the trigger pin is low
    digitalWrite(trigger_pin, LOW);
    delayMicroseconds(2);

    // Send out a short burst to detect
    digitalWrite(trigger_pin, HIGH);
    delayMicroseconds(10); // Added this line

    // Cancel the pulse and wait for the reply
    digitalWrite(trigger_pin, LOW);
    float duration = (float) pulseIn(echo_pin, HIGH, max_ms) / 2.f;

    if (duration == 0) return (float) max_ms / 2.f;

    runningDuration.add(duration);
    return runningDuration.getMedian();
}


#include "VoltageSensor.h"
#include <Arduino.h>
#include <MathExtra.h>

#define ARDUINO_MAX_VOLTAGE 5.f  // Volts

VoltageSensor::VoltageSensor(uint8_t pin, int r1, int r2) {
    this->pin = pin;
    this->max_voltage = ARDUINO_MAX_VOLTAGE * (float) ((r1 + r2) / r2);
}

float VoltageSensor::read() {
    voltage.add(map((float) analogRead(pin), 0., 1023., 0, max_voltage));
    return voltage.getMedian();
}
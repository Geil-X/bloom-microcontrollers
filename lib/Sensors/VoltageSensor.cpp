#include "VoltageSensor.h"
#include <Arduino.h>
#include "MathExtra.h"

#define ARDUINO_MAX_VOLTAGE 5.f  // Volts

VoltageSensor::VoltageSensor(uint8_t pin, int r1, int r2, float threshold_voltage) {
    this->pin = pin;
    this-> threshold_voltage = threshold_voltage;
    this->max_voltage = ARDUINO_MAX_VOLTAGE * (float) ((float) (r1 + r2) / (float) r2);
}

float VoltageSensor::read() {
    runningVoltage.add(map((float) analogRead(pin), 0., 1023., 0, max_voltage));
    return runningVoltage.getMedian();
}

bool VoltageSensor::hasPower() {
    float voltage = read();
    if (voltage > threshold_voltage) {
        has_power = true;
        return true;
    }
    return false;
}

bool VoltageSensor::lostPower() {
    float voltage = read();
    if (has_power && voltage < threshold_voltage) {
        has_power = false;
        return true;
    }
    return false;
}

bool VoltageSensor::gainedPower() {
    float voltage = read();
    if (!has_power && voltage > threshold_voltage) {
        has_power = true;
        return true;
    }
    return false;
}

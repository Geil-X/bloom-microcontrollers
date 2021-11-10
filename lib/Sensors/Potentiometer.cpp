#include "Potentiometer.h"

#define ANALOG_IN_MAX 1023

Potentiometer::Potentiometer(uint8_t potentiometer_pin, int low, int high) {
    this->potentiometer_pin = potentiometer_pin;
    this->low = low;
    this->high = high;
}

int Potentiometer::value() {
    int reading =  (int) map(analogRead(potentiometer_pin), 0, ANALOG_IN_MAX, high, low);
    runningMedian.add((float) reading);
    return (int) runningMedian.getMedian();
}


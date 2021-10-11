#include "PotentiometerButton.h"

#define ANALOG_IN_MAX 1023

PotentiometerButton::PotentiometerButton(uint8_t button_pin, uint8_t potentiometer_pin, int low, int high) : Button(button_pin) {
    this->potentiometer_pin = potentiometer_pin;
    this->low = low;
    this->high = high;
}

int PotentiometerButton::value() const {
    return (int) map(analogRead(potentiometer_pin), 0, ANALOG_IN_MAX, high, low);
}


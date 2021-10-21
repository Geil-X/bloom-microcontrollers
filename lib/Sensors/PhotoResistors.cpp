#include "PhotoResistor.h"

PhotoResistor::PhotoResistor(uint8_t pin) {
    this->pin = pin;
    pinMode(pin, INPUT);
}

int PhotoResistor::read() const {
    return analogRead(pin);
}

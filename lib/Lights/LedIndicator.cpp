#include "LedIndicator.h"

#include <Arduino.h>

LedIndicator::LedIndicator(uint8_t pin) {
    this->pin = pin;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}


void LedIndicator::blink(unsigned long on_time_ms, unsigned long off_time_ms) {
    this->on_time = on_time_ms;
    this->off_time = off_time_ms;
    start_time = millis();
}

void LedIndicator::blinkBlocking(unsigned long on_time_ms, unsigned long off_time_ms, int count) {
    blink(on_time_ms, off_time_ms);
    while (count > 0) {
        if (update()) { count--; }
    }
    off();
}

void LedIndicator::on() {
    this->off_time = 0;
    this->on_time = 0;
    this->pin_state = HIGH;
    digitalWrite(pin, HIGH);
    start_time = millis();
}

void LedIndicator::off() {
    this->off_time = 0;
    this->on_time = 0;
    this->pin_state = LOW;
    digitalWrite(pin, LOW);
    start_time = millis();
}

bool LedIndicator::update() {
    if (on_time == 0 || off_time == 0) return false;
    unsigned long ms = millis();
    unsigned long time_since_start = ms - start_time;

    if (pin_state == HIGH && time_since_start > on_time) {
        pin_state = LOW;
        digitalWrite(pin, LOW);
        start_time = ms;
    } else if (pin_state == LOW && time_since_start > off_time) {
        pin_state = HIGH;
        digitalWrite(pin, HIGH);
        start_time = ms;
        return true;
    }
    return false;
}

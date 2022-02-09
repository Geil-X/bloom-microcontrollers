#include <Arduino.h>

#include <LedIndicator.h>

#define LED_PIN 14

LedIndicator indicator(14);

void setup() {
    indicator.blinkBlocking(50, 50, 5);
    indicator.blink(50, 50);
}

void loop() {
    indicator.update();
}
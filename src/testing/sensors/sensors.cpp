#include <Arduino.h>
#include <TMCStepper.h>
#include <AccelStepper.h>
#include <DipSwitch.h>

#define PIN_COUNT 7

const int pins[PIN_COUNT]{5, 6, 7, 8, 9, 10, 11};
DipSwitch<PIN_COUNT> dipSwitch = DipSwitch<PIN_COUNT>(pins);

uint8_t previousReading = 0;

void setup() {
    Serial.begin(9600);

}

void loop() {
    uint8_t reading = dipSwitch.value();

    if (previousReading != reading) {
        Serial.println("Dip switched: " + String(reading));
        previousReading = reading;
    }

    delay(100);
}
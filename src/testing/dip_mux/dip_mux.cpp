#include <Arduino.h>
#include <AccelStepper.h>
#include <TMCStepper.h>
#include <Wire.h>
#include <MathExtra.h>
#include <MuxDipSwitch.h>
#include <Button.h>

MuxDipSwitch dipSwitch(5, 2, 3, 4);
Button button(6);

void setup() {
    Serial.begin(115200);
}

void loop() {
    if (button.isPressed()) {
        Serial.println("Dip Switch Value: " + String(dipSwitch.read()));
    }
}
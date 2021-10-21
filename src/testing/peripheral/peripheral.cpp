#include <Arduino.h>
#include <I2CPeripheral.h>

void setup() {
    I2CPeripheral::join(16);
}

void loop() {
    delay(100);
}
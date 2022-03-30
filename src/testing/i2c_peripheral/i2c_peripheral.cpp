#include <Arduino.h>
#include <I2CPeripheral.h>

struct Transmission {
    __attribute__((unused)) uint8_t buttonId = 0;
} transmission;

#define DEVICE_ID 8

void setup() {
    Serial.begin(115200);
    Serial.println("Running I2C Peripheral Test");

    I2CPeripheral::begin(DEVICE_ID);
}

void loop() {
}
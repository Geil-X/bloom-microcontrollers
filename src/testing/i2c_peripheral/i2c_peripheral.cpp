#include <Arduino.h>
#include <I2CPeripheral.h>
#include <MorningGlory.h>
#include "Logging.h"

Flower flower = Flower(EN, DIR, STEP, SS, SDI, SDO, SCK);
I2CPeripheral i2CPeripheral;

#define DEVICE_ID 8

void setup() {
    Log::connect(Log::DEBUG);
    Log::info("Running I2C Peripheral Test");

    i2CPeripheral.begin(DEVICE_ID);
}

void loop() {
}
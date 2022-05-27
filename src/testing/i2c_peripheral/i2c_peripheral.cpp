#include <Arduino.h>
#include <I2cPeripheral.h>
#include <MorningGlory.h>
#include "Logging.h"

Flower flower = Flower(EN, DIR, STEP, SS, SDI, SDO, SCK);
I2cPeripheral i2cPeripheral;

#define DEVICE_ID 8

void setup() {
    Log::connect(Log::DEBUG);
    Log::info("Running I2c Peripheral Test");

    i2cPeripheral.begin(DEVICE_ID);
}

void loop() {
    i2cPeripheral.executeCommand(flower);
}
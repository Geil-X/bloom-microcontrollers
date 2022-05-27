#include <Arduino.h>
#include <I2cController.h>

#include <Logging.h>
#include <MorningGlory.h>
#include <LedIndicator.h>

#define DEVICE_ID 8

I2cController i2cCommunication;
LedIndicator indicator(IND_PIN);

void setup() {
    Log::connect(Log::DEBUG, 115200);
    Log::info("Running I2c Controller Test");
}

void loop() {
    delay(500);
    indicator.on();
    i2cCommunication.sendOpenPacket(DEVICE_ID);
    indicator.off();

    delay(500);
    indicator.on();
    i2cCommunication.sendClosePacket(DEVICE_ID);
    indicator.off();

    delay(500);
    indicator.on();
    i2cCommunication.sendOpenToPacket(random16(), DEVICE_ID);
    indicator.off();

    delay(500);
    indicator.on();
    i2cCommunication.sendSpeedPacket(random16(), DEVICE_ID);
    indicator.off();

    delay(500);
    indicator.on();
    i2cCommunication.sendAccelerationPacket(random16(), DEVICE_ID);
    indicator.off();

    delay(500);
}
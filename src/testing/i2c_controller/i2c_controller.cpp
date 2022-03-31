#include <Arduino.h>
#include <I2CController.h>

#define DEVICE_ID 8

I2CController i2cCommunication;

void setup() {
    Serial.begin(115200);
    Serial.println("Running I2C Controller Test");
}

void loop() {
    delay(500);
    i2cCommunication.sendOpenPacket(DEVICE_ID);
    delay(500);
    i2cCommunication.sendClosePacket(DEVICE_ID);
    delay(500);
    i2cCommunication.sendOpenToPacket(random16(), DEVICE_ID);
    delay(500);
    i2cCommunication.sendSpeedPacket(random16(), DEVICE_ID);
    delay(500);
    i2cCommunication.sendAccelerationPacket(random16(), DEVICE_ID);
    delay(500);
}
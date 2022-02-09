#include <Arduino.h>
#include <Wire.h>
#include <I2CTransfer.h>
#include <SerialTransfer.h>

struct TransferPacket {
    __attribute__((unused)) uint16_t data;
} transferPacket;

SerialTransfer serialInput;
I2CTransfer i2cOutput;

void setup() {
    Serial.begin(115200);
    serialInput.begin(Serial);
    Wire.begin();
    i2cOutput.begin(Wire);
}

void loop() {
    if (Serial.available() != 0) {
        serialInput.rxObj(transferPacket);
        i2cOutput.txObj(transferPacket);
    }
}
#include <Arduino.h>
#include <CommandPacket.h>
#include "I2CController.h"

#define IND_PIN 14 // PC0

#define CONTROLLER_PACKET_SIZE COMMAND_PACKET_SIZE + 1
struct ControllerPacket {
    union {
        // Parameter access
        struct {
            uint8_t i2cAddress;
            CommandPacket commandPacket;
        };

        // Byte array access. This length needs to be updates as fields are added
        uint8_t arr[CONTROLLER_PACKET_SIZE];
    };
} controllerPacket;

I2CController i2cCommunication;

void setup() {
    Serial.begin(115200);
    pinMode(IND_PIN, OUTPUT);
    digitalWrite(IND_PIN, LOW);
}

void loop() {
    if (!Serial.available() || Serial.available() < CONTROLLER_PACKET_SIZE) return;
    digitalWrite(IND_PIN, HIGH);
    Serial.readBytes(controllerPacket.arr, CONTROLLER_PACKET_SIZE);
    i2cCommunication.sendPacket(controllerPacket.commandPacket, controllerPacket.i2cAddress);
    digitalWrite(IND_PIN, LOW);
}
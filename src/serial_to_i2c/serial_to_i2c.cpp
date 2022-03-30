#include <Arduino.h>
#include <CommandPacket.h>
#include "I2CController.h"

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
    while (!Serial) {}
}

void loop() {
}

__attribute__((unused)) void serialEvent() {
    if (!Serial.available()) return;
    Serial.readBytes(controllerPacket.arr, CONTROLLER_PACKET_SIZE);
    i2cCommunication.sendPacket(controllerPacket.commandPacket, controllerPacket.i2cAddress);
}
#include <Arduino.h>


#include <Types.h>
#include <Flower.h>
#include <I2cController.h>
#include <Logging.h>

#define SERIAL_PACKET_SIZE (COMMAND_PACKET_SIZE + 1)
struct SerialPacket {
    union {
        struct {
            I2cAddress i2cAddress;
            CommandPacket commandPacket;
        };
        uint8_t arr[SERIAL_PACKET_SIZE];
    };
} serialPacket;

I2cController controller;


void setup() {
    Log::connect(Log::DEBUG);
    Log::debug("Starting Serial TTL to I2C Converter");
    controller.scan();
}

void loop() {
    delay(2000);
    controller.scan();
    delay(2000);
    controller.sendOpenPacket(17);
    delay(2000);
    controller.sendClosePacket(17);
}

/**
 * On receiving a serial communication, send the packet through the i2c address
 * over the i2c bus.
 */
void serialEvent() {
    while (Serial.available()) {
        Serial.readBytes(serialPacket.arr, SERIAL_PACKET_SIZE);
        controller.sendPacket(serialPacket.commandPacket, serialPacket.i2cAddress);
    }
}

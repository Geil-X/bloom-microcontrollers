#include <Arduino.h>

#include <Types.h>
#include <Flower.h>
#include <I2cController.h>
#include <Logging.h>


SerialCommandPacket serialCommandPacket;
SerialResponsePacket serialResponsePacket;
I2cController controller;

void setup() {
    Log::connect(Log::PRINT);
    Log::debug("Starting Serial TTL to I2C Converter");
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

}

void loop() {
}

/**
 * On receiving a serial communication, send the packet through the i2c address
 * over the i2c bus.
 */
void serialEvent() {
    while (Serial.available()) {
        digitalWrite(LED_BUILTIN, HIGH);
        Serial.readBytes(serialCommandPacket.arr, SERIAL_PACKET_SIZE);
        if (serialCommandPacket.commandPacket.commandId == Command::PING) {
            serialResponsePacket.responsePacket = controller.request(serialCommandPacket.i2cAddress);
            serialResponsePacket.i2cAddress = serialCommandPacket.i2cAddress;
            Serial.write(serialResponsePacket.arr, SERIAL_RESPONSE_PACKET_SIZE);
        } else {
            controller.sendPacket(serialCommandPacket.commandPacket, serialCommandPacket.i2cAddress);
            digitalWrite(LED_BUILTIN, LOW);
        }
    }
}

#include "I2CController.h"

#include <FastLED.h>
#include <Wire.h>

I2CController::I2CController() {
    Wire.begin();
}

void I2CController::sendSetupPacket(uint8_t targetAddress) {
    clearPacket();
    packet.commandId = Command::SETUP;
    sendPacket(targetAddress);
}

void I2CController::sendHomePacket(uint8_t targetAddress) {
    clearPacket();
    packet.commandId = Command::HOME;
    sendPacket(targetAddress);
}

void I2CController::sendOpenPacket(uint8_t targetAddress) {
    clearPacket();
    packet.commandId = Command::OPEN;
    sendPacket(targetAddress);
}

void I2CController::sendClosePacket(uint8_t targetAddress) {
    clearPacket();
    packet.commandId = Command::CLOSE;
    sendPacket(targetAddress);
}

void I2CController::sendOpenToPacket(fract16 percentage, uint8_t targetAddress) {
    clearPacket();
    packet.commandId = Command::OPEN_TO;
    packet.percentage = percentage;
    sendPacket(targetAddress);
}

void I2CController::sendSpeedPacket(uint16_t speed, uint8_t targetAddress) {
    clearPacket();
    packet.commandId = Command::SPEED;
    packet.speed = speed;
    sendPacket(targetAddress);
}

void I2CController::sendAccelerationPacket(uint16_t acceleration, uint8_t targetAddress) {
    clearPacket();
    packet.commandId = Command::ACCELERATION;
    packet.acceleration = acceleration;
    sendPacket(targetAddress);
}

void I2CController::sendPacket(CommandPacket &commandPacket, uint8_t targetAddress) {
    packet = commandPacket;
    sendPacket(targetAddress);
}

void I2CController::clearPacket() {
    for (unsigned char &i: packet.arr) {
        i = 0;
    }
}

void I2CController::sendPacket(uint8_t targetAddress) {
    Wire.beginTransmission(targetAddress);
    for (unsigned char &i: packet.arr) {
        Serial.print(String(i));
    }
    Serial.println();
    Wire.write(packet.arr, COMMAND_PACKET_SIZE);
    Wire.endTransmission();
}
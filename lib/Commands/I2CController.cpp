#include "I2CController.h"

#include <FastLED.h>
#include <SerialTransfer.h>

I2CController::I2CController(SerialTransfer &serialTransfer) : serialTransfer(serialTransfer) {}

void I2CController::sendSetupPacket() {
    packet.commandId = Command::SETUP;
    sendPacket();
}

void I2CController::sendHomePacket() {
    packet.commandId = Command::HOME;
    sendPacket();
}

void I2CController::sendOpenPacket() {
    packet.commandId = Command::OPEN;
    sendPacket();
}

void I2CController::sendClosePacket() {
    packet.commandId = Command::CLOSE;
    sendPacket();
}

void I2CController::sendOpenToPacket(fract16 percentage) {
    packet.commandId = Command::OPEN_TO;
    packet.percentage = percentage;
    sendPacket();
}

void I2CController::sendSpeedPacket(uint16_t speed) {
    packet.commandId = Command::SPEED;
    packet.speed = speed;
    sendPacket();
}

void I2CController::sendAccelerationPacket(uint16_t acceleration) {
    packet.commandId = Command::ACCELERATION;
    packet.acceleration = acceleration;
    sendPacket();
}

void I2CController::sendPacket() {
    serialTransfer.txObj(packet);
}
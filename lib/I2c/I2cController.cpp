#include "I2cController.h"
#include "Types.h"

#include <FastLED.h>
#include <Wire.h>

#include <Logging.h>

I2cController::I2cController() {
    Wire.begin();
}

void I2cController::sendSetupPacket(uint8_t targetAddress) {
    clearPacket();
    commandPacket.commandId = Command::SETUP;
    sendPacket(targetAddress);
    Log::debug("Sent Setup Packet");
}

void I2cController::sendHomePacket(uint8_t targetAddress) {
    clearPacket();
    commandPacket.commandId = Command::HOME;
    sendPacket(targetAddress);
    Log::debug("Sent Home Packet");
}

void I2cController::sendOpenPacket(uint8_t targetAddress) {
    Log::debug("Clearing Packet");
    clearPacket();
    commandPacket.commandId = Command::OPEN;
    Log::debug("Sending Open Packet");
    sendPacket(targetAddress);
    Log::debug("Sent Open Packet");
}

void I2cController::sendClosePacket(uint8_t targetAddress) {
    clearPacket();
    commandPacket.commandId = Command::CLOSE;
    sendPacket(targetAddress);
    Log::debug("Sent Close Packet");
}

void I2cController::sendOpenToPacket(fract16 percentage, uint8_t targetAddress) {
    clearPacket();
    commandPacket.commandId = Command::OPEN_TO;
    commandPacket.percentage = percentage;
    sendPacket(targetAddress);
    Log::debug("Sent Open To Packet: " + String(percentage));
}

void I2cController::sendSpeedPacket(uint16_t speed, uint8_t targetAddress) {
    clearPacket();
    commandPacket.commandId = Command::SPEED;
    commandPacket.speed = speed;
    sendPacket(targetAddress);
    Log::debug("Sent Speed Packet: " + String(speed));
}

void I2cController::sendAccelerationPacket(uint16_t acceleration, uint8_t targetAddress) {
    clearPacket();
    commandPacket.commandId = Command::ACCELERATION;
    commandPacket.acceleration = acceleration;
    sendPacket(targetAddress);
    Log::debug("Sent Acceleration Packet: " + String(acceleration));
}

void I2cController::sendPacket(CommandPacket &packet, I2cAddress targetAddress) {
    commandPacket = packet;
    sendPacket(targetAddress);
}

void I2cController::clearPacket() {
    for (unsigned char &i: commandPacket.arr) {
        i = 0;
    }
}

void I2cController::scan(uint8_t maxAddress) {
    Log::debug("Clearing stored i2c addresses");
    connectedI2cAddresses.clear();
    for (byte i2cAddress = 8; i2cAddress < maxAddress; i2cAddress++) {
        Wire.beginTransmission(i2cAddress);
        if (Wire.endTransmission() == 0) {
            connectedI2cAddresses.add(i2cAddress);
            Log::debug("Found I2C address: " + String(i2cAddress));
        }
    }
}

void I2cController::sendPacket(uint8_t targetAddress) {
    Wire.beginTransmission(targetAddress);
    Wire.write(commandPacket.arr, COMMAND_PACKET_SIZE);
    Wire.endTransmission();
}

ResponsePacket &I2cController::request(uint8_t targetAddress) {
    Wire.requestFrom(targetAddress, (uint8_t) RESPONSE_PACKET_SIZE);
    Wire.readBytes(responsePacket.arr, RESPONSE_PACKET_SIZE);
    return responsePacket;
}

Set &I2cController::connectedAddresses() {
    return connectedI2cAddresses;
}

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
    packet.commandId = Command::SETUP;
    sendPacket(targetAddress);
    Log::debug("Sent Setup Packet");
}

void I2cController::sendHomePacket(uint8_t targetAddress) {
    clearPacket();
    packet.commandId = Command::HOME;
    sendPacket(targetAddress);
    Log::debug("Sent Home Packet");
}

void I2cController::sendOpenPacket(uint8_t targetAddress) {
    Log::debug("Clearing Packet");
    clearPacket();
    packet.commandId = Command::OPEN;
    Log::debug("Sending Open Packet");
    sendPacket(targetAddress);
    Log::debug("Sent Open Packet");
}

void I2cController::sendClosePacket(uint8_t targetAddress) {
    clearPacket();
    packet.commandId = Command::CLOSE;
    sendPacket(targetAddress);
    Log::debug("Sent Close Packet");
}

void I2cController::sendOpenToPacket(fract16 percentage, uint8_t targetAddress) {
    clearPacket();
    packet.commandId = Command::OPEN_TO;
    packet.percentage = percentage;
    sendPacket(targetAddress);
    Log::debug("Sent Open To Packet: " + String(percentage));
}

void I2cController::sendSpeedPacket(uint16_t speed, uint8_t targetAddress) {
    clearPacket();
    packet.commandId = Command::SPEED;
    packet.speed = speed;
    sendPacket(targetAddress);
    Log::debug("Sent Speed Packet: " + String(speed));
}

void I2cController::sendAccelerationPacket(uint16_t acceleration, uint8_t targetAddress) {
    clearPacket();
    packet.commandId = Command::ACCELERATION;
    packet.acceleration = acceleration;
    sendPacket(targetAddress);
    Log::debug("Sent Acceleration Packet: " + String(acceleration));
}

void I2cController::sendPacket(CommandPacket &commandPacket, I2cAddress targetAddress) {
    packet = commandPacket;
    sendPacket(targetAddress);
}

void I2cController::clearPacket() {
    for (unsigned char &i: packet.arr) {
        i = 0;
    }
}

void I2cController::scan(uint8_t maxAddress) {
    Log::debug("Clearing stored i2c addresses");
    connectedI2cAddresses.clear();
    for (byte i2cAddress = 8; i2cAddress < maxAddress; i2cAddress++)
    {
        Wire.beginTransmission (i2cAddress);
        if (Wire.endTransmission () == 0)
        {
            connectedI2cAddresses.add(i2cAddress);
            Log::debug("Found I2C address: " + String(i2cAddress));
        }
    }
}

void I2cController::sendPacket(uint8_t targetAddress) {
    if (connectedI2cAddresses.has(targetAddress)) {
        Log::debug("Start");
        Wire.beginTransmission(targetAddress);
        Log::debug("Write array");
        Wire.write(packet.arr, COMMAND_PACKET_SIZE);
        Log::debug("End Transmission");
        Wire.endTransmission();
        Log::debug("Done");
    }
    else {
        Log::error("Can't send message because I2C device is not connected: " + String(targetAddress));
    }
}
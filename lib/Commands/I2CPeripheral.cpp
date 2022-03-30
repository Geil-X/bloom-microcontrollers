#include "I2CPeripheral.h"

#include <Wire.h>

#include <Command.h>

CommandPacket I2CPeripheral::packet{};

void I2CPeripheral::begin(uint8_t deviceId) { // NOLINT(readability-convert-member-functions-to-static)
    Wire.begin(deviceId);
    Wire.onReceive(printCommandFromWire);
}

bool I2CPeripheral::executeCommandFromWire(Flower &flower) {
    if (!Wire.available()) return false;
    Wire.readBytes(packet.arr, COMMAND_PACKET_SIZE);

    switch (packet.commandId) {
        case Command::NO_COMMAND: {
            return false;
        }
        case Command::SETUP: {
            Command::setup(flower);
            break;
        }
        case Command::HOME: {
            Command::home(flower);
            break;
        }
        case Command::OPEN: {
            Command::open(flower);
            break;
        }
        case Command::CLOSE: {
            Command::close(flower);
            break;
        }
        case Command::OPEN_TO: {
            Command::openTo(flower, packet.percentage);
            break;
        }
        case Command::SPEED: {
            Command::speed(flower, packet.speed);
            break;
        }
        case Command::ACCELERATION: {
            Command::acceleration(flower, packet.acceleration);
            break;
        }
        default: {
            return false;
        }
    }
    return true;
}


void I2CPeripheral::printCommandFromWire(int packetSize) {
    if (packetSize == 0 || !Wire.available()) return;
    if (packetSize > COMMAND_PACKET_SIZE) {
        Serial.println("Expected a size of " + String(COMMAND_PACKET_SIZE) + " but got " + String(packetSize));
    }
    Wire.readBytes(packet.arr, COMMAND_PACKET_SIZE);

    switch (packet.commandId) {
        case Command::NO_COMMAND: {
            Serial.println("No Command");
            return;
        }
        case Command::SETUP: {
            Serial.println("Setup");
            return;
        }
        case Command::HOME: {
            Serial.println("Home");
            return;
        }
        case Command::OPEN: {
            Serial.println("Open");
            return;
        }
        case Command::CLOSE: {
            Serial.println("Close");
            return;
        }
        case Command::OPEN_TO: {
            Serial.println("Open To: " + String(packet.percentage));
            return;
        }
        case Command::SPEED: {
            Serial.println("Speed: " + String(packet.speed));
            return;
        }
        case Command::ACCELERATION: {
            Serial.println("Acceleration: " + String(packet.acceleration));
            return;
        }
        default: {
            Serial.println("Unknown Command Id: " + String(packet.commandId));
            return;
        }
    }
}
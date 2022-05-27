#include "I2cPeripheral.h"
#include "Logging.h"

#include <Wire.h>
#include "Command.h"
#include <Flower.h>

I2cPeripheral *I2cPeripheral::peripheral = nullptr;

void I2cPeripheral::begin(I2cAddress deviceId, Flower*newFlower) {
    flower = newFlower;
    Wire.begin(deviceId);
    peripheral = this;
    Wire.onReceive(
            [](int bytes) {
                I2cPeripheral::peripheral->readCommandFromWire(bytes);
            }
    );
    Wire.onRequest(
            [] {
                I2cPeripheral::peripheral->sendResponseThroughWire();
            }
            );
}

void I2cPeripheral::readCommandFromWire(int bytes) {
    if (bytes == 0 || !Wire.available()) return;
    if (bytes < COMMAND_PACKET_SIZE) {
        while (Wire.available()) { Wire.read(); }
        return;
    }
    Wire.readBytes(commandPacket.arr, COMMAND_PACKET_SIZE);
    if (flower != nullptr) {
        flower->executeCommand()
    }
}

void I2cPeripheral::sendResponseThroughWire() {
    if (flower != nullptr) {
        responsePacket.time = millis();
        responsePacket.position = flower->getPosition();
        responsePacket.target = flower->getTarget();
        responsePacket.acceleration = flower->getAcceleration();
        responsePacket.speed = flower->getSpeed();

        Wire.write(responsePacket.arr, RESPONSE_PACKET_SIZE);
    }
}


void I2cPeripheral::executeCommand() {
    switch (commandPacket.commandId) {
        case Command::NO_COMMAND: {
            return;
        }
        case Command::SETUP: {
            Log::debug("Running setup sequence");
//            Command::setup(flower);
            break;
        }
        case Command::HOME: {
            Log::debug("Running home sequence");
//            Command::home(flower);
            break;
        }
        case Command::OPEN: {
            Log::debug("Opening flower");
//            Command::open(flower);
            break;
        }
        case Command::CLOSE: {
            Log::debug("Closing flower");
//            Command::close(flower);
            break;
        }
        case Command::OPEN_TO: {
            Log::debug("Opening flower to " + String(commandPacket.percentage));
//            Command::openTo(flower, packet.percentage);
            break;
        }
        case Command::SPEED: {
            Log::debug("Setting flower speed to " + String(commandPacket.speed));
//            Command::speed(flower, packet.speed);
            break;
        }
        case Command::ACCELERATION: {
            Log::debug("Setting flower acceleration to " + String(commandPacket.acceleration));
//            Command::acceleration(flower, packet.acceleration);
            break;
        }
    }
    commandPacket.commandId = Command::NO_COMMAND;
}
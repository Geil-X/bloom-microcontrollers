#include <I2cPeripheral.h>

#include <Wire.h>

#include <Types.h>
#include <Flower.h>

I2cPeripheral *I2cPeripheral::peripheral = nullptr;

void I2cPeripheral::begin(I2cAddress deviceId, Flower *newFlower) {
    flower = newFlower;
    Wire.begin(deviceId);
    peripheral = this;
    Wire.onReceive([](int bytes) {
        I2cPeripheral::peripheral->readCommandFromWire(bytes);
    });
    Wire.onRequest([] {
        I2cPeripheral::peripheral->sendResponseThroughWire();
    });
}

void I2cPeripheral::readCommandFromWire(int bytes) {
    if (bytes == 0 || !Wire.available()) return;
    if (bytes < COMMAND_PACKET_SIZE) {
        while (Wire.available()) { Wire.read(); }
        return;
    }
    Wire.readBytes(commandPacket.arr, COMMAND_PACKET_SIZE);
    if (flower != nullptr) {
        flower->executeCommand(commandPacket);
    }
}

void I2cPeripheral::sendResponseThroughWire() {
    if (flower == nullptr) return;

    responsePacket.time = millis();
    responsePacket.position = flower->getPosition();
    responsePacket.target = flower->getTarget();
    responsePacket.acceleration = flower->getAcceleration();
    responsePacket.speed = flower->getSpeed();

    Wire.write(responsePacket.arr, RESPONSE_PACKET_SIZE);
}

void I2cPeripheral::executeCommand() {
    if (flower == nullptr) return;

    switch (commandPacket.commandId) {
        case Command::NO_COMMAND: {
            return;
        }
        case Command::SETUP: {
            flower->setup();
            break;
        }
        case Command::HOME: {
            flower->home();
            break;
        }
        case Command::OPEN: {
            flower->openAsync();
            break;
        }
        case Command::CLOSE: {
            flower->closeAsync();
            break;
        }
        case Command::OPEN_TO: {
            flower->openToAsync(commandPacket.percentage);
            break;
        }
        case Command::SPEED: {
            flower->setSpeed(commandPacket.speed);
            break;
        }
        case Command::ACCELERATION: {
            flower->setAcceleration(commandPacket.acceleration);
            break;
        }
    }

    // Clear the command packet so the command is only run once
    commandPacket.commandId = Command::NO_COMMAND;
}
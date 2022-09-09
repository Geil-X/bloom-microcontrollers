#include <I2cPeripheral.h>

#include <Wire.h>

#include <Types.h>
#include <Flower.h>
#include <Logging.h>

I2cPeripheral *I2cPeripheral::peripheral = nullptr;

void I2cPeripheral::begin(I2cAddress deviceId, Flower *newFlower) {
    flower = newFlower;
    Wire.begin(deviceId);
    peripheral = this;
    commandPacket.commandId = Command::NO_COMMAND;
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
        case Command::PING: {
            break;
        }
        case Command::SETUP: {
            Log::debug("Running setup sequence");
            flower->setup();
            break;
        }
        case Command::HOME: {
            Log::debug("Running home sequence");
            Log::debug("But isn't because it's disabled");
            // TODO: this is hanging up even when not hit in switch?
            // this could be caused by the fact that the home function is
            // synchronous
//            flower->home();
            break;
        }
        case Command::OPEN: {
            Log::debug("Opening flower");
            flower->openAsync();
            break;
        }
        case Command::CLOSE: {
            Log::debug("Closing flower");
            flower->closeAsync();
            break;
        }
        case Command::OPEN_TO: {
            Log::debug("Opening flower to " + String(commandPacket.percentage));
            flower->openToAsync(commandPacket.percentage);
            break;
        }
        case Command::SPEED: {
            Log::debug("Setting flower speed to " + String(commandPacket.speed));
            flower->setMaxSpeed(commandPacket.speed);
            break;
        }
        case Command::ACCELERATION: {
            Log::debug("Setting flower acceleration to " + String(commandPacket.acceleration));
            flower->setAcceleration(commandPacket.acceleration);
            break;
        }
    }

    commandPacket.commandId = Command::NO_COMMAND;
}
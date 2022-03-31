#include "I2CPeripheral.h"
#include "Logging.h"

#include <Wire.h>
#include <Command.h>

I2CPeripheral *I2CPeripheral::peripheral = nullptr;


void I2CPeripheral::begin(uint8_t deviceId) {
    Wire.begin(deviceId);
    peripheral = this;
    Wire.onReceive(
            [](int bytes) {
                I2CPeripheral::peripheral->readCommandFromWire(bytes);
            }
    );
}

void I2CPeripheral::readCommandFromWire(int bytes) {
    if (bytes == 0 || !Wire.available()) return;
    if (bytes < COMMAND_PACKET_SIZE) {
        while (Wire.available()) { Wire.read(); }
        return;
    }
    Wire.readBytes(packet.arr, COMMAND_PACKET_SIZE);
}


void I2CPeripheral::executeCommand(Flower &flower) {
    switch (packet.commandId) {
        case Command::NO_COMMAND: {
            return;
        }
        case Command::SETUP: {
            Log::debug("Running setup sequence");
            Command::setup(flower);
            break;
        }
        case Command::HOME: {
            Log::debug("Running home sequence");
            Command::home(flower);
            break;
        }
        case Command::OPEN: {
            Log::debug("Opening flower");
            Command::open(flower);
            break;
        }
        case Command::CLOSE: {
            Log::debug("Closing flower");
            Command::close(flower);
            break;
        }
        case Command::OPEN_TO: {
            Log::debug("Opening flower to " + String(packet.percentage));
            Command::openTo(flower, packet.percentage);
            break;
        }
        case Command::SPEED: {
            Log::debug("Setting flower speed to " + String(packet.speed));
            Command::speed(flower, packet.speed);
            break;
        }
        case Command::ACCELERATION: {
            Log::debug("Setting flower acceleration to " + String(packet.acceleration));
            Command::acceleration(flower, packet.acceleration);
            break;
        }
    }
    packet.commandId = Command::NO_COMMAND;
}
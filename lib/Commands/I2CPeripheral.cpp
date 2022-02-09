#include "I2CPeripheral.h"

#include <SerialTransfer.h>

#include <Command.h>

I2CPeripheral::I2CPeripheral(SerialTransfer &serialTransfer)
        : serialTransfer(serialTransfer) {
}

bool I2CPeripheral::executeCommandFromWire(Flower &flower) {
    serialTransfer.rxObj(packet);

    switch (packet.commandId) {
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
        default:
            return false;
    }
    return true;
}
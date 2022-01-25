#include <I2CPeripheral.h>

#include <I2CCommandFactory.h>
#include <Wire.h>

Packet I2CPeripheral::packet = {};

void I2CPeripheral::join(int device) {
    Wire.begin(device);
    Wire.onReceive(receiveCommand);
}

void I2CPeripheral::receiveCommand(int size) {
    packet = I2CCommandFactory::getPacketFromWire(size);
}

/**
 * We need to clear the volatile command when returning a copy of that command.
 */
bool I2CPeripheral::executeCommand(Flower &flower) {
    if (I2CCommandFactory::isNoCommand(packet)) return false;

    I2CCommandFactory::executePacket(packet, flower);
    packet = I2CCommandFactory::noCommandPacket();
    return true;
}

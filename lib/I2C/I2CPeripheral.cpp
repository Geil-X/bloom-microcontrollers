#include <I2CPeripheral.h>

#include <I2CCommandFactory.h>
#include <Wire.h>

volatile Command *I2CPeripheral::command = nullptr;

void I2CPeripheral::join(int device) {
    Wire.begin(device);
    Wire.onReceive(receiveCommand);
    Wire.onRequest(requestCommand);
}

void I2CPeripheral::receiveCommand(int size) {
    command = I2CCommandFactory::commandFromWire(size);
}

/**
 * We need to clear the volatile command when returning a copy of that command.
 */
Command *I2CPeripheral::tryGetCommand() {
    if (command == nullptr) { return nullptr; }

    Command *returnCommand = command->copy();
    delete command;
    command = nullptr;

    return returnCommand;
}

void I2CPeripheral::requestCommand() {
    Packets packets = I2CCommandFactory::createPacket(command);
    Wire.write(packets.data, MAX_I2C_DATA_SIZE);
}

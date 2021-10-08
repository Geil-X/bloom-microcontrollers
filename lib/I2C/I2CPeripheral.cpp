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

void I2CPeripheral::requestCommand() {
    Packets packets = I2CCommandFactory::createPacket(command);
    Wire.write(packets.data, MAX_I2C_DATA_SIZE);
}

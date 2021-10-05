#include <I2CPeripheral.h>

#include <I2CCommandFactory.h>
#include <Wire.h>

int I2CPeripheral::device_id = 0;
Command *I2CPeripheral::command = nullptr;

void I2CPeripheral::join(int device) {
    Wire.begin(device);
    Wire.onReceive(receiveCommand);
    Wire.onRequest(requestCommand);
    I2CPeripheral::device_id = device;
}

void I2CPeripheral::receiveCommand(int size) {
    command = I2CCommandFactory::commandFromWire(size);
}

void I2CPeripheral::requestCommand() {
    Packets packets = I2CCommandFactory::createPacket(command);
    Wire.beginTransmission(device_id);
    Wire.write(packets.data, MAX_I2C_DATA_SIZE);
    Wire.endTransmission();
}

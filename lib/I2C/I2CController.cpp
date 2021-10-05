#include <I2CController.h>

void I2CController::join () {
    Wire.begin();
}

void I2CController::sendCommand(int device, Command* command) {
    Packets packets = I2CCommandFactory::createPacket(command);
    Wire.beginTransmission(device);
    Wire.write(packets.data, MAX_I2C_DATA_SIZE);
    Wire.endTransmission();
}

Command * I2CController::requestCommand(int device) {
    Wire.requestFrom(device, MAX_I2C_DATA_SIZE);
    return I2CCommandFactory::commandFromWire(MAX_I2C_DATA_SIZE);
}

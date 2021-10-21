#include <I2CController.h>

void I2CController::join () {
    Wire.begin();
}

void I2CController::sendPacket(int device, const Packet& packet) {
    Wire.beginTransmission(device);
    Wire.write(packet.data, MAX_I2C_DATA_SIZE);
    Wire.endTransmission();
}

Packet I2CController::requestPacket(int device) {
    Wire.requestFrom(device, MAX_I2C_DATA_SIZE);
    return I2CCommandFactory::getPacketFromWire(MAX_I2C_DATA_SIZE);
}
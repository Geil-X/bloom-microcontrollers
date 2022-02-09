#ifndef FLOWER_I2CPERIPHERAL_H
#define FLOWER_I2CPERIPHERAL_H

#include <SerialTransfer.h>

#include <I2CPacket.h>
#include <Flower.h>

class I2CPeripheral {
public:
    explicit I2CPeripheral(SerialTransfer &transfer);
    bool executeCommandFromWire(Flower &flower);
private:
    I2CPacket packet{};
    SerialTransfer &serialTransfer;
};


#endif //FLOWER_I2CPERIPHERAL_H

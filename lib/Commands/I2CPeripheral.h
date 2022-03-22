#ifndef FLOWER_I2CPERIPHERAL_H
#define FLOWER_I2CPERIPHERAL_H

#include <SerialTransfer.h>

#include <CommandPacket.h>
#include <Flower.h>

class I2CPeripheral {
public:
    explicit I2CPeripheral(SerialTransfer &transfer);
    bool executeCommandFromWire(Flower &flower);
private:
    CommandPacket packet{};
    SerialTransfer &serialTransfer;
};


#endif //FLOWER_I2CPERIPHERAL_H

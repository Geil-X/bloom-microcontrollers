#ifndef FLOWER_I2CPERIPHERAL_H
#define FLOWER_I2CPERIPHERAL_H

#include <SerialTransfer.h>

#include <CommandPacket.h>
#include <Flower.h>

class I2CPeripheral {
public:
    void begin(uint8_t deviceId);
    void executeCommand(Flower &flower);
private:
    void readCommandFromWire(int bytes);
    CommandPacket packet{};
    static I2CPeripheral *peripheral;
};


#endif //FLOWER_I2CPERIPHERAL_H

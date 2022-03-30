#ifndef FLOWER_I2CPERIPHERAL_H
#define FLOWER_I2CPERIPHERAL_H

#include <SerialTransfer.h>

#include <CommandPacket.h>
#include <Flower.h>

class I2CPeripheral {
public:
    static void begin(uint8_t device_id);
    static bool executeCommandFromWire(Flower &flower);
    static void printCommandFromWire(int packetSize);
private:
    static CommandPacket packet;
};


#endif //FLOWER_I2CPERIPHERAL_H

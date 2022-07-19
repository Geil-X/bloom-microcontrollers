#ifndef FLOWER_I2CPERIPHERAL_H
#define FLOWER_I2CPERIPHERAL_H

#include <SerialTransfer.h>

#include <Flower.h>

class I2cPeripheral {
public:
    void begin(I2cAddress deviceId, Flower*newFlower);
    void executeCommand();
private:
    // Functions are used within I2C ISR so they aren't used directly in code
    __attribute__((unused)) void readCommandFromWire(int bytes);
    __attribute__((unused)) void sendResponseThroughWire();

    CommandPacket commandPacket{};
    ResponsePacket responsePacket{};
    static I2cPeripheral *peripheral;
    Flower *flower = nullptr;
};

#endif //FLOWER_I2CPERIPHERAL_H

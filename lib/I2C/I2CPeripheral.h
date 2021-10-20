#ifndef FLOWER_I2CPERIPHERAL_H
#define FLOWER_I2CPERIPHERAL_H

#include <Command.h>
#include <I2CCommandFactory.h>

class I2CPeripheral {
public:
    /**
     * Join the I2C communication channel as the peripheral device under the given device id.
     * @param device The device id to join the I2C bus with
     */
    static void join(int device);

    static void executeCommand(Flower &flower);

private:
    /**
     * Receive a command through the I2C bus and assign it to the #command variable.
     */
    static void receiveCommand(int);

    static Packet packet;
};

#endif //FLOWER_I2CPERIPHERAL_H
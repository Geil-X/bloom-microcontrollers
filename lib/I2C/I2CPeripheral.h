#ifndef FLOWER_I2CPERIPHERAL_H
#define FLOWER_I2CPERIPHERAL_H

#include <Command.h>

class I2CPeripheral {
public:
    /**
     * Join the I2C communication channel as the peripheral device under the given device id.
     * @param device The device id to join the I2C bus with
     */
    static void join(int device);

    /**
     * Receive a command through the I2C bus and assign it to the #command variable.
     */
    static void receiveCommand(int);

    /**
     * Send the currently held command to the controller when requested.
     */
    static void requestCommand();

    static int device_id;
    static Command *command;
};

#endif //FLOWER_I2CPERIPHERAL_H
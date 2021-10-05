#ifndef FLOWER_I2CCONTROLLER_H
#define FLOWER_I2CCONTROLLER_H

#include <Wire.h>
#include <Command.h>
#include <I2CCommandFactory.h>

class I2CController {
public:
    /**
     * Join the I2C communication channel as the controller.
     */
    static void join();

    /**
     * Send a command to a peripheral device which is uniquely identified by the device id.
     * @param device The peripheral device id to send the command to.
     * @param command The command being sent to the peripheral device.
     */
    static void sendCommand(int device, Command *command);

    /**
     * Request the current command held by the device being queried.
     * @param device The device to request the command from.
     * @return The command currently held by the queried device.
     */
    static Command *requestCommand(int device);
};

#endif //FLOWER_I2CCONTROLLER_H

#ifndef FLOWER_I2CCONTROLLER_H
#define FLOWER_I2CCONTROLLER_H

#include <FastLED.h>
#include <SerialTransfer.h>

#include <I2CPacket.h>

class I2CController {
public:
    I2CController(SerialTransfer&serialTransfer);

    void sendSetupPacket();
    void sendHomePacket();
    void sendOpenPacket();
    void sendClosePacket();
    void sendOpenToPacket(fract16 percentage);
    void sendSpeedPacket(uint16_t speed);
    void sendAccelerationPacket(uint16_t acceleration);

private:
    void sendPacket();

    I2CPacket packet;
    SerialTransfer serialTransfer;
};

#endif //FLOWER_I2CCONTROLLER_H

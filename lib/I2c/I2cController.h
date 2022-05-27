#ifndef FLOWER_I2CCONTROLLER_H
#define FLOWER_I2CCONTROLLER_H

#include <FastLED.h>
#include <I2cTransfer.h>
#include "Types.h"

class I2cController {
public:
    I2cController();

    void sendSetupPacket(I2cAddress targetAddress);
    void sendHomePacket(I2cAddress targetAddress);
    void sendOpenPacket(I2cAddress targetAddress);
    void sendClosePacket(I2cAddress targetAddress);
    void sendOpenToPacket(Percentage percentage, I2cAddress targetAddress);
    void sendSpeedPacket(Speed speed, I2cAddress targetAddress);
    void sendAccelerationPacket(Acceleration acceleration, I2cAddress targetAddress);
    void sendPacket(CommandPacket & commandPacket, I2cAddress targetAddress);

private:
    void clearPacket();
    void sendPacket(uint8_t targetAddress);

    CommandPacket packet{};
};

#endif //FLOWER_I2CCONTROLLER_H

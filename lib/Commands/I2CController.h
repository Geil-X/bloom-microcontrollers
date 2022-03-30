#ifndef FLOWER_I2CCONTROLLER_H
#define FLOWER_I2CCONTROLLER_H

#include <FastLED.h>
#include <I2CTransfer.h>

#include <CommandPacket.h>

class I2CController {
public:
    I2CController();

    void sendSetupPacket(uint8_t targetAddress);
    void sendHomePacket(uint8_t targetAddress);
    void sendOpenPacket(uint8_t targetAddress);
    void sendClosePacket(uint8_t targetAddress);
    void sendOpenToPacket(fract16 percentage, uint8_t targetAddress);
    void sendSpeedPacket(uint16_t speed, uint8_t targetAddress);
    void sendAccelerationPacket(uint16_t acceleration, uint8_t targetAddress);
    void sendPacket(CommandPacket & commandPacket, uint8_t targetAddress);

private:
    void clearPacket();
    void sendPacket(uint8_t targetAddress);

    CommandPacket packet{};
};

#endif //FLOWER_I2CCONTROLLER_H

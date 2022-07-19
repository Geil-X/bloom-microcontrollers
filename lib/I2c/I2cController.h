#ifndef FLOWER_I2CCONTROLLER_H
#define FLOWER_I2CCONTROLLER_H

#include <FastLED.h>
#include <set.h>
#include <I2cTransfer.h>

#include <Types.h>

class I2cController {
public:
    I2cController();

    /**
     * Scan through the first 100 I2C addresses to see which devices are
     * connected. These I2C addresses are registered to this controller
     * to ensure that the I2C communications remain stable.
     */
    void scan(uint8_t maxAddress=127);

    // Sending Data
    void sendSetupPacket(I2cAddress targetAddress);
    void sendHomePacket(I2cAddress targetAddress);
    void sendOpenPacket(I2cAddress targetAddress);
    void sendClosePacket(I2cAddress targetAddress);
    void sendOpenToPacket(Percentage percentage, I2cAddress targetAddress);
    void sendSpeedPacket(Speed speed, I2cAddress targetAddress);
    void sendAccelerationPacket(Acceleration acceleration, I2cAddress targetAddress);
    void sendPacket(CommandPacket & commandPacket, I2cAddress targetAddress);

    // Requesting Data
    ResponsePacket& request(uint8_t targetAddress);

private:
    void clearPacket();
    void sendPacket(uint8_t targetAddress);

    CommandPacket commandPacket{};
    ResponsePacket responsePacket{};
    Set connectedI2cAddresses;
};

#endif //FLOWER_I2CCONTROLLER_H

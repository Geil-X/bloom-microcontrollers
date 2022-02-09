#ifndef FLOWER_I2CPACKET_H
#define FLOWER_I2CPACKET_H

#include <Command.h>

struct I2CPacket {
    Command::Id commandId;
    union {
        // "Open" & "Open To" Commands
        fract16 percentage;

        // Set Speed
        uint16_t speed;

        // Set Acceleration
        uint16_t acceleration;
    };
};

#endif //FLOWER_I2CPACKET_H

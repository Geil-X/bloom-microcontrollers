#ifndef FLOWER_COMMANDPACKET_H
#define FLOWER_COMMANDPACKET_H

#include <Command.h>

struct CommandPacket {
    Command::Id commandId;
    union {
        // "Open" & "Open To" Commands
        fract16 percentage;

        // Set Speed
        uint16_t speed;

        // Set Acceleration
        uint16_t acceleration;
    };
} commandPacket{};

#endif //FLOWER_COMMANDPACKET_H

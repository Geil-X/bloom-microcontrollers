#ifndef FLOWER_COMMANDPACKET_H
#define FLOWER_COMMANDPACKET_H

#include <Command.h>

#define COMMAND_PACKET_SIZE 3

struct CommandPacket {
    union {
        struct {
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
        uint8_t arr[COMMAND_PACKET_SIZE];
    };
};

#endif //FLOWER_COMMANDPACKET_H

#ifndef FLOWER_I2CCOMMANDFACTORY_H
#define FLOWER_I2CCOMMANDFACTORY_H

#include <Arduino.h>
#include <Command.h>

// This tells the I2C controllers how long the expected message should be.
#define MAX_I2C_DATA_SIZE 3

struct Packets {
    unsigned char data[MAX_I2C_DATA_SIZE];
};

struct Packet_uint16_t {
    uint8_t byte1;  // Left 8 bits, NNNNNNNNxxxxxxxx
    uint8_t byte2;  // Right 8 bits, xxxxxxxxNNNNNNNN
};

class I2CCommandFactory {
public:

    static Command *parsePackets(Packets packets);

    static Packets createPacket(volatile Command *command);

    static Command *commandFromWire(int packet_size);

    static Packet_uint16_t uint16ToPacket(uint16_t value);

    static uint16_t packetToUint16(uint8_t byte1, uint8_t byte2);
};

#endif //FLOWER_I2CCOMMANDFACTORY_H

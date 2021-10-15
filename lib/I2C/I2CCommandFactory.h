#ifndef FLOWER_I2CCOMMANDFACTORY_H
#define FLOWER_I2CCOMMANDFACTORY_H

#include <Arduino.h>
#include <Command.h>

// This tells the I2C controllers how long the expected message should be.
#define MAX_I2C_DATA_SIZE 5

union FloatConversion {
    float f;
    uint32_t i;
};

struct Packet {
    uint8_t data[MAX_I2C_DATA_SIZE];
};

struct UInt16Packet {
    uint8_t byte1;
    uint8_t byte2;
};

struct UInt32Packet {
    uint8_t byte1;
    uint8_t byte2;
    uint8_t byte3;
    uint8_t byte4;
};

class I2CCommandFactory {
public:

    static Command *parsePacket(Packet packet);

    static Packet createPacket(volatile Command *command);

    static Command *commandFromWire(int packet_size);


private:
    static UInt16Packet toPacket(int value);
    static UInt32Packet toPacket(float value);

    static int packetToInt(uint8_t byte1, uint8_t byte2);
    static float packetToFloat(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4);

    static void writeToPacket(unsigned char value, Packet& packet, int position);
    static void writeToPacket(int value, Packet& packet, int position);
    static void writeToPacket(float value, Packet& packet, int position);
};

#endif //FLOWER_I2CCOMMANDFACTORY_H

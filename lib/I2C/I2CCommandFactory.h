#ifndef FLOWER_I2CCOMMANDFACTORY_H
#define FLOWER_I2CCOMMANDFACTORY_H

#include <Command.h>

// This tells the I2C controllers how long the expected message should be.
#define MAX_I2C_DATA_SIZE 2

struct Packets {
    unsigned char data[MAX_I2C_DATA_SIZE];
};

class I2CCommandFactory {
public:
    static Command *parsePackets(Packets packets);

    static Packets createPacket(Command *command);

    static Command *commandFromWire(int packet_size);
};

#endif //FLOWER_I2CCOMMANDFACTORY_H

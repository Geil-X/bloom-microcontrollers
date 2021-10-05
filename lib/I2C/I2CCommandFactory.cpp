#include <I2CCommandFactory.h>

#include <Wire.h>

Command *I2CCommandFactory::parsePackets(Packets packets) {
    auto commandId = (CommandId) packets.data[0];
    switch (commandId) {
        case SETUP:
            return new Setup();
        case HOME:
            return new Home();
        case OPEN:
            return new Open();
        case CLOSE:
            return new Close();
        case OPEN_TO:
            return new OpenTo(packets.data[1]);
        case SPEED:
            return new Speed(packets.data[1]);
        case ACCELERATION:
            return new Acceleration(packets.data[1]);
        case PING:
            return new Ping();
        case NO_COMMAND:
            return new NoCommand();
        default:
            return new InvalidCommand();
    }
}

Packets I2CCommandFactory::createPacket(Command *command) {
    Packets packets = {};
    if (command == nullptr) {
        packets.data[0] = NO_COMMAND;
        return packets;
    }

    switch (command->id) {
        case OPEN_TO: {
            auto *openTo = (OpenTo *) command;
            packets.data[0] = openTo->id;
            packets.data[1] = openTo->percentage;
            break;
        }
        case SPEED: {
            auto speed = (Speed *) command;
            packets.data[0] = speed->id;
            packets.data[1] = speed->speed;
            break;
        }
        case ACCELERATION: {
            auto acceleration = (Acceleration *) command;
            packets.data[0] = acceleration->id;
            packets.data[1] = acceleration->acceleration;
            break;
        }
        case NO_COMMAND:
        case SETUP:
        case HOME:
        case OPEN:
        case CLOSE:
        case PING:
        case INVALID_COMMAND:
            packets.data[0] = (unsigned char) command->id;
            break;
    }

    return packets;
}

Command *I2CCommandFactory::commandFromWire(int packet_size) {
    Packets packets = {};
    int packet_index = 0;
    while (Wire.available() && packet_index < packet_size && packet_index < MAX_I2C_DATA_SIZE) {
        packets.data[packet_index] = Wire.read();
        packet_index++;
    }
    return parsePackets(packets);
}

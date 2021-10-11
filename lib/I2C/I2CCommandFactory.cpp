#include <I2CCommandFactory.h>

#include <Wire.h>

Command *I2CCommandFactory::parsePackets(Packets packets) {
    auto commandId = (Command::Id) packets.data[0];
    switch (commandId) {
        case Command::SETUP:
            return new Setup();
        case Command::HOME:
            return new Home();
        case Command::OPEN:
            return new Open();
        case Command::CLOSE:
            return new Close();
        case Command::OPEN_TO:
            return new OpenTo(packetToUint16(packets.data[1], packets.data[2]));
        case Command::SPEED:
            return new Speed(packetToUint16(packets.data[1], packets.data[2]));
        case Command::ACCELERATION:
            return new Acceleration(packetToUint16(packets.data[1], packets.data[2]));
        case Command::PING:
            return new Ping();
        case Command::NO_COMMAND:
            return new NoCommand();
        default:
            return new InvalidCommand();
    }
}

Packets I2CCommandFactory::createPacket(volatile Command *command) {
    Packets packets = {};
    if (command == nullptr) {
        packets.data[0] = Command::INVALID_COMMAND;
        return packets;
    }

    switch (command->id) {
        case Command::OPEN_TO: {
            auto *openTo = (OpenTo *) command;
            Packet_uint16_t percentagePacket = uint16ToPacket(openTo->percentage);
            packets.data[0] = openTo->id;
            packets.data[1] = percentagePacket.byte1;
            packets.data[2] = percentagePacket.byte2;
            break;
        }
        case Command::SPEED: {
            auto speed = (Speed *) command;
            Packet_uint16_t speedPacket = uint16ToPacket(speed->speed);
            packets.data[0] = speed->id;
            packets.data[1] = speedPacket.byte1;
            packets.data[2] = speedPacket.byte2;
            break;
        }
        case Command::ACCELERATION: {
            auto acceleration = (Acceleration *) command;
            Packet_uint16_t accelerationPacket = uint16ToPacket(acceleration->acceleration);
            packets.data[0] = acceleration->id;
            packets.data[1] = accelerationPacket.byte1;
            packets.data[2] = accelerationPacket.byte2;
            break;
        }
        case Command::NO_COMMAND:
        case Command::SETUP:
        case Command::HOME:
        case Command::OPEN:
        case Command::CLOSE:
        case Command::PING:
        case Command::INVALID_COMMAND:
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

Packet_uint16_t I2CCommandFactory::uint16ToPacket(uint16_t value) {
    return Packet_uint16_t{(uint8_t) (value >> 8), (uint8_t) value};
}

uint16_t I2CCommandFactory::packetToUint16(uint8_t byte1, uint8_t byte2) {
    return
            (((uint16_t) byte1) << 8) +
            ((uint16_t) byte2);
}

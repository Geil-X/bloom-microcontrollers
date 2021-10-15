#include <I2CCommandFactory.h>

#include <Wire.h>

Command *I2CCommandFactory::parsePacket(Packet packet) {
    auto commandId = (Command::Id) packet.data[0];
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
            return new OpenTo(packetToFloat(
                    packet.data[1], packet.data[2], packet.data[3], packet.data[4]));
        case Command::SPEED:
            return new Speed(packetToInt(packet.data[1], packet.data[2]));
        case Command::ACCELERATION:
            return new Acceleration(packetToInt(packet.data[1], packet.data[2]));
        case Command::PING:
            return new Ping();
        case Command::NO_COMMAND:
            return new NoCommand();
        default:
            return new InvalidCommand();
    }
}

Packet I2CCommandFactory::createPacket(volatile Command *command) {
    Packet packet = {};
    if (command == nullptr) {
        packet.data[0] = Command::INVALID_COMMAND;
        return packet;
    }
    auto commandId = (unsigned char) command->id;
    switch (command->id) {
        case Command::OPEN_TO: {
            auto *openTo = (OpenTo *) command;
            writeToPacket(commandId, packet, 0);
            writeToPacket(openTo->percentage, packet, 1);
            break;
        }
        case Command::SPEED: {
            auto speed = (Speed *) command;
            writeToPacket(commandId, packet, 0);
            writeToPacket(speed->speed, packet, 1);
            break;
        }
        case Command::ACCELERATION: {
            auto acceleration = (Acceleration *) command;
            writeToPacket(commandId, packet, 0);
            writeToPacket(acceleration->acceleration, packet, 1);
            break;
        }
        case Command::NO_COMMAND:
        case Command::SETUP:
        case Command::HOME:
        case Command::OPEN:
        case Command::CLOSE:
        case Command::PING:
        case Command::INVALID_COMMAND:
            writeToPacket(commandId, packet, 0);
            break;
    }

    return packet;
}

Command *I2CCommandFactory::commandFromWire(int packet_size) {
    Packet packets = {};
    int packet_index = 0;
    while (Wire.available() && packet_index < packet_size && packet_index < MAX_I2C_DATA_SIZE) {
        packets.data[packet_index] = Wire.read();
        packet_index++;
    }
    return parsePacket(packets);
}

UInt16Packet I2CCommandFactory::toPacket(int value) {
    return UInt16Packet{(uint8_t) (value >> 8), (uint8_t) value};
}

UInt32Packet I2CCommandFactory::toPacket(float value) {
    FloatConversion floatToInt{};
    floatToInt.f = value;

    return UInt32Packet{
            (uint8_t) (floatToInt.i >> 24),
            (uint8_t) (floatToInt.i >> 16),
            (uint8_t) (floatToInt.i >> 8),
            (uint8_t) floatToInt.i
    };
}

int I2CCommandFactory::packetToInt(uint8_t byte1, uint8_t byte2) {
    return
            (((uint16_t) byte1) << 8) +
            ((uint16_t) byte2);
}

float I2CCommandFactory::packetToFloat(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4) {
    FloatConversion intToFloat{};
    intToFloat.i =
            (((uint32_t) byte1) << 24) +
            (((uint32_t) byte2) << 16) +
            (((uint32_t) byte3) << 8) +
            ((uint32_t) byte4);

    return intToFloat.f;
}

void I2CCommandFactory::writeToPacket(unsigned char value, Packet &packet, int position) {
    packet.data[position] = value;
}

void I2CCommandFactory::writeToPacket(int value, Packet &packet, int position) {
    UInt16Packet floatPacket = toPacket(value);
    packet.data[position] = floatPacket.byte1;
    packet.data[position + 1] = floatPacket.byte2;
}

void I2CCommandFactory::writeToPacket(float value, Packet &packet, int position) {
    UInt32Packet floatPacket = toPacket(value);
    packet.data[position] = floatPacket.byte1;
    packet.data[position + 1] = floatPacket.byte2;
    packet.data[position + 2] = floatPacket.byte3;
    packet.data[position + 3] = floatPacket.byte4;
}

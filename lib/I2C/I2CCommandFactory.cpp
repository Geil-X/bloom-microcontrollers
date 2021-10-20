#include <I2CCommandFactory.h>

#include <Wire.h>

Packet I2CCommandFactory::setupPacket() {
    Packet packet = {};
    writeToPacket((uint8_t) Command::SETUP, packet, 0);
    return packet;
}

Packet I2CCommandFactory::homePacket() {
    Packet packet = {};
    writeToPacket((uint8_t) Command::HOME, packet, 0);
    return packet;
}

Packet I2CCommandFactory::openPacket() {
    Packet packet = {};
    writeToPacket(Command::OPEN, packet, 0);
    return packet;
}

Packet I2CCommandFactory::closePacket() {
    Packet packet = {};
    writeToPacket(Command::CLOSE, packet, 0);
    return packet;
}

Packet I2CCommandFactory::openToPacket(float percentage) {
    Packet packet = {};
    writeToPacket((uint8_t) Command::OPEN_TO, packet, 0);
    writeToPacket(percentage, packet, 1);
    return packet;
}

Packet I2CCommandFactory::speedPacket(int speed) {
    Packet packet = {};
    writeToPacket(Command::SPEED, packet, 0);
    writeToPacket(speed, packet, 1);
    return packet;
}

Packet I2CCommandFactory::accelerationPacket(int acceleration) {
    Packet packet = {};
    writeToPacket(Command::ACCELERATION, packet, 0);
    writeToPacket(acceleration, packet, 1);
    return packet;
}

Packet I2CCommandFactory::noCommandPacket() {
    Packet packet = {};
    writeToPacket(Command::NO_COMMAND, packet, 0);
    return packet;
}

bool I2CCommandFactory::isNoCommand(Packet &packet) {
    return packet.data[0] == Command::NO_COMMAND;
}

void I2CCommandFactory::executePacket(Packet packet, Flower &flower) {
    auto commandId = (Command::Id) packet.data[0];
    switch (commandId) {
        case Command::SETUP: {
            Command::setup(flower);
            break;
        }
        case Command::HOME: {
            Command::home(flower);
            break;
        }
        case Command::OPEN: {
            Command::open(flower);
            break;
        }
        case Command::CLOSE: {
            Command::close(flower);
            break;
        }
        case Command::OPEN_TO: {
            float percentage = packetToFloat(
                    packet.data[1], packet.data[2],
                    packet.data[3], packet.data[4]);
            Command::openTo(flower, percentage);
            break;
        }
        case Command::SPEED: {
            int speed = packetToInt(packet.data[1], packet.data[2]);
            Command::speed(flower, speed);
            break;
        }
        case Command::ACCELERATION: {
            int acceleration = packetToInt(packet.data[1], packet.data[2]);
            Command::acceleration(flower, acceleration);
            break;
        }
        default:
            break;
    }
}

Packet I2CCommandFactory::getPacketFromWire(int packet_size) {
    Packet packet = {};
    int packet_index = 0;
    while (Wire.available() && packet_index < packet_size && packet_index < MAX_I2C_DATA_SIZE) {
        packet.data[packet_index] = Wire.read();
        packet_index++;
    }
    return packet;
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

void I2CCommandFactory::writeToPacket(uint8_t value, Packet &packet, int position) {
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


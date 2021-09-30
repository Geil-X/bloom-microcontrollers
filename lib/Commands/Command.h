#ifndef COMMANDS_COMMAND_H
#define COMMANDS_COMMAND_H

// This tells the I2C controllers how long the expected message should be.
#define MAX_I2C_DATA_SIZE 2

// ---- Command Ids ----

enum CommandId : unsigned char {
    NO_COMMAND,
    SETUP,
    HOME,
    OPEN,
    CLOSE,
    OPEN_TO,
    SPEED,
    ACCELERATION,
    PING,
    INVALID_COMMAND
};

// ---- Abstract Base Class ----

class Command {
public:
    Command(CommandId id) { this->id = id; }

    friend bool operator==(const Command &lhs, const Command &rhs) {
        return lhs.id == rhs.id;
    }

    friend bool operator!=(const Command &lhs, const Command &rhs) {
        return !(lhs == rhs);
    }

    CommandId id;
};


// ---- Error Commands & Null Case ----

class NoCommand : public Command {
public:
    NoCommand() : Command(NO_COMMAND) {}
};

class InvalidCommand : public Command {
public:
    InvalidCommand() : Command(INVALID_COMMAND) {}
};

// ---- Commands ----

class Setup : public Command {
public:
    Setup() : Command(SETUP) {}
};

class Home : public Command {
public:
    Home() : Command(HOME) {}
};

class Open : public Command {
public:
    Open() : Command(OPEN) {}
};

class Close : public Command {
public:
    Close() : Command(CLOSE) {}
};

class OpenTo : public Command {
public:
    explicit OpenTo(unsigned char percentage) : Command(OPEN_TO) {
        this->percentage = percentage;
    }

    unsigned char percentage;
};

class Speed : public Command {
public:
    explicit Speed(unsigned char speed) : Command(SPEED) {
        this->speed = speed;
    }

    unsigned char speed;
};

class Acceleration : public Command {
public:
    explicit Acceleration(unsigned char acceleration) : Command(ACCELERATION) {
        this->acceleration = acceleration;
    }

    unsigned char acceleration;
};

class Ping : public Command {
public:
    Ping() : Command(PING) {}
};

// ---- Parsing ----

Command *parsePacket(unsigned char *packets, int packet_count) {
    if (packet_count <= 0) return new NoCommand();

    CommandId commandId = (CommandId) packets[0];
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
            return new OpenTo(packets[1]);
        case SPEED:
            return new Speed(packets[1]);
        case ACCELERATION:
            return new Acceleration(packets[1]);
        case PING:
            return new Ping();
        case NO_COMMAND:
            return new NoCommand();
        case INVALID_COMMAND:
            return new InvalidCommand();
        default:
            return new InvalidCommand();
    }
}

// ---- Creation ----

unsigned char *createPacket(Command command) {
    unsigned char[MAX_I2C_DATA_SIZE] * packet;
    switch (command.id) {
        case OPEN_TO:
            OpenTo openTo = (OpenTo) command;
            packet[0] = openTo.id;
            packet[1] = openTo.percentage;
            break;
        case SPEED:
            Speed speed = (Speed) command;
            packet[0] = openTo.id;
            packet[1] = openTo.speed;
            break;
        case ACCELERATION:
            Acceleration acceleration = (Acceleration) command;
            packet[0] = openTo.id;
            packet[1] = openTo.acceleration;
            break;
        case NO_COMMAND:
        case SETUP:
        case HOME:
        case OPEN:
        case CLOSE:
        case PING:
        case INVALID_COMMAND:
            packet[0] = command.id;
            break;
    }

    return packet;
}

#endif // COMMANDS_COMMAND_H

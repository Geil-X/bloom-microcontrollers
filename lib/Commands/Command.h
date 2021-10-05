#ifndef COMMANDS_COMMAND_H
#define COMMANDS_COMMAND_H


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
    explicit Command(CommandId id) { this->id = id; }

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

    friend bool operator==(const Speed &lhs, const Speed &rhs) {
        return lhs.id == rhs.id && lhs.speed == rhs.speed;
    }

    unsigned char speed;
};

class Acceleration : public Command {
public:
    explicit Acceleration(unsigned char acceleration) : Command(ACCELERATION) {
        this->acceleration = acceleration;
    }

    friend bool operator==(const Acceleration &lhs, const Acceleration &rhs) {
        return lhs.id == rhs.id && lhs.acceleration == rhs.acceleration;
    }

    unsigned char acceleration;
};

class Ping : public Command {
public:
    Ping() : Command(PING) {}
};

#endif  // COMMANDS_COMMAND_H
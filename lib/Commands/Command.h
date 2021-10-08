#ifndef COMMANDS_COMMAND_H
#define COMMANDS_COMMAND_H

#include <Flower.h>

// ---- Abstract Base Class ----

class Command {
public:
    enum Id : unsigned char {
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

    explicit Command(Id id) { this->id = id; }

    virtual String toString() = 0;

    virtual void execute(Flower &flower) = 0;

    friend bool operator==(const Command &lhs, const Command &rhs) {
        return lhs.id == rhs.id;
    }

    friend bool operator!=(const Command &lhs, const Command &rhs) {
        return !(lhs == rhs);
    }

    Id id;
};

// ---- Error Commands & Null Case ----

class NoCommand : public Command {
public:
    NoCommand() : Command(NO_COMMAND) {}

    void execute(Flower &flower) override {}

    String toString() override { return "No Command"; }
};

class InvalidCommand : public Command {
public:
    InvalidCommand() : Command(INVALID_COMMAND) {}

    void execute(Flower &flower) override {}

    String toString() override { return "Invalid Command"; }
};

// ---- Commands ----

class Setup : public Command {
public:
    Setup() : Command(SETUP) {}

    void execute(Flower &flower) override { flower.setup(); }

    String toString() override { return "Setup"; }

};

class Home : public Command {
public:
    Home() : Command(HOME) {}

    void execute(Flower &flower) override { flower.home(); }

    String toString() override { return "Home"; }
};

class Open : public Command {
public:
    Open() : Command(OPEN) {}

    void execute(Flower &flower) override { flower.open(); }

    String toString() override { return "Open"; }
};

class Close : public Command {
public:
    Close() : Command(CLOSE) {}

    void execute(Flower &flower) override { flower.close(); }

    String toString() override { return "Close"; }
};

class OpenTo : public Command {
public:
    explicit OpenTo(unsigned char percentage) : Command(OPEN_TO) {
        this->percentage = percentage;
    }

    void execute(Flower &flower) override { flower.openTo(percentage); }

    String toString() override { return "Open To: " + String(percentage); }

    unsigned char percentage;
};

class Speed : public Command {
public:
    explicit Speed(unsigned char speed) : Command(SPEED) {
        this->speed = speed;
    }

    void execute(Flower &flower) override { flower.setMaxSpeed(speed); }

    String toString() override { return "Speed: " + String(speed); }

    friend bool operator==(const Speed &lhs, const Speed &rhs) {
        return lhs.id == rhs.id && lhs.speed == rhs.speed;
    }

    friend bool operator!=(const Speed &lhs, const Speed &rhs) {
        return !(lhs == rhs);
    }

    unsigned char speed;
};

class Acceleration : public Command {
public:
    explicit Acceleration(unsigned char acceleration) : Command(ACCELERATION) {
        this->acceleration = acceleration;
    }

    void execute(Flower &flower) override { flower.setAcceleration(acceleration); }

    String toString() override { return "Acceleration: " + String(acceleration); }

    friend bool operator==(const Acceleration &lhs, const Acceleration &rhs) {
        return lhs.id == rhs.id && lhs.acceleration == rhs.acceleration;
    }

    friend bool operator!=(const Acceleration &lhs, const Acceleration &rhs) {
        return !(lhs == rhs);
    }

    unsigned char acceleration;
};

class Ping : public Command {
public:
    Ping() : Command(PING) {}

    void execute(Flower &flower) override { }

    String toString() override { return "Ping"; }
};

#endif  // COMMANDS_COMMAND_H
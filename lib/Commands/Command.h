#ifndef COMMANDS_COMMAND_H
#define COMMANDS_COMMAND_H

#include <Flower.h>
#include <Logging.h>

// ---- Abstract Base Class ----

/**
 * A command object used for communicating and executing commands on a flower device.
 */
class Command {
public:
    enum Id : int {
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

    virtual ~Command() = default;

    virtual Command *copy() volatile = 0;

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

    Command *copy() volatile override { return new NoCommand(); }

    void execute(Flower &flower) override { debug(toString()); }

    String toString() override { return "No Command"; }
};

class InvalidCommand : public Command {
public:
    InvalidCommand() : Command(INVALID_COMMAND) {}

    ~InvalidCommand() override = default;

    Command *copy() volatile override { return new InvalidCommand(); }

    void execute(Flower &flower) override { debug(toString()); }

    String toString() override { return "Invalid Command"; }
};

// ---- Commands ----

class Setup : public Command {
public:
    Setup() : Command(SETUP) {}

    ~Setup() override = default;

    Command *copy() volatile override { return new Setup(); }

    void execute(Flower &flower) override {
        debug(toString());
        flower.setup();
    }

    String toString() override { return "Setup"; }

};

class Home : public Command {
public:
    Home() : Command(HOME) {}

    ~Home() override = default;

    Command *copy() volatile override { return new Home(); }

    void execute(Flower &flower) override {
        debug(toString());
        flower.home();
    }

    String toString() override { return "Home"; }
};

class Open : public Command {
public:
    Open() : Command(OPEN) {}

    ~Open() override = default;

    Command *copy() volatile override { return new Open(); }

    void execute(Flower &flower) override {
        debug(toString());
        flower.openAsync();
    }

    String toString() override { return "Open"; }
};

class Close : public Command {
public:
    Close() : Command(CLOSE) {}

    ~Close() override = default;

    Command *copy() volatile override { return new Close(); }

    void execute(Flower &flower) override {
        debug(toString());
        flower.closeAsync();
    }

    String toString() override { return "Close"; }
};

class OpenTo : public Command {
public:
    explicit OpenTo(int percentage) : Command(OPEN_TO) {
        this->percentage = constrain(percentage, 0, 100);
    }

    ~OpenTo() override = default;

    Command *copy() volatile override { return new OpenTo(percentage); }

    void execute(Flower &flower) override {
        debug(toString());
        flower.openToAsync((float) percentage);
    }

    String toString() override { return "Open To: " + String(percentage); }

    int percentage;
};

class Speed : public Command {
public:
    explicit Speed(int speed) : Command(SPEED) {
        this->speed = speed;
    }

    ~Speed() override = default;

    Command *copy() volatile override { return new Speed(speed); }

    void execute(Flower &flower) override {
        debug(toString());
        flower.setMaxSpeed((float) speed);
    }

    String toString() override { return "Speed: " + String(speed); }

    friend bool operator==(const Speed &lhs, const Speed &rhs) {
        return lhs.id == rhs.id && lhs.speed == rhs.speed;
    }

    friend bool operator!=(const Speed &lhs, const Speed &rhs) {
        return !(lhs == rhs);
    }

    int speed;
};

class Acceleration : public Command {
public:
    explicit Acceleration(int acceleration) : Command(ACCELERATION) {
        this->acceleration = acceleration;
    }

    ~Acceleration() override = default;

    Command *copy() volatile override { return new Acceleration(acceleration); }

    void execute(Flower &flower) override {
        debug(toString());
        flower.setAcceleration((float) acceleration);
    }

    String toString() override { return "Acceleration: " + String(acceleration); }

    friend bool operator==(const Acceleration &lhs, const Acceleration &rhs) {
        return lhs.id == rhs.id && lhs.acceleration == rhs.acceleration;
    }

    friend bool operator!=(const Acceleration &lhs, const Acceleration &rhs) {
        return !(lhs == rhs);
    }

    int acceleration;
};

class Ping : public Command {
public:
    Ping() : Command(PING) {}

    ~Ping() override = default;

    Command *copy() volatile override { return new Ping(); }

    void execute(Flower &flower) override { debug(toString()); }

    String toString() override { return "Ping"; }
};

#endif  // COMMANDS_COMMAND_H
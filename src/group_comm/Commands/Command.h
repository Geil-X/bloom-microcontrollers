#ifndef COMMANDS_COMMAND_H
#define COMMANDS_COMMAND_H

#include <Arduino.h>
#include <FlowerGroup.h>
#include "logging.h"


// ---- Abstract Base Class ----

class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;

    friend bool operator==(const Command &lhs, const Command &rhs) {
        return typeid(lhs).name() == typeid(rhs).name();
    }

    friend bool operator!=(const Command &lhs, const Command &rhs) {
        return !(lhs == rhs);
    }

    static bool isAll(const int *ids) {
        return ids[0] == 0;
    }
};


// ---- Error Commands & Null Case ----

class InvalidCommand : public Command {
public:
    String commandName;

    explicit InvalidCommand(const String &commandName) {
        this->commandName = commandName;
    }

    ~InvalidCommand() override = default;

    void execute() override {
        log(ERROR, "No command named: \"" + commandName + "\"");
    }
};

class InvalidDigitCast : public Command {
public:
    String userInput;

    explicit InvalidDigitCast(const String &userInput) {
        this->userInput = userInput;
    }

    ~InvalidDigitCast() override = default;

    void execute() override {
        log(ERROR, "Could not cast user input value \"" + userInput + "\" into an integer.");
    }
};

class InvalidArgumentCount : public Command {
public:
    String commandName;

    explicit InvalidArgumentCount(const String &commandName) {
        this->commandName = commandName;
    }

    ~InvalidArgumentCount() override = default;

    void execute() override {
        log(ERROR, "No command named: \"" + commandName + "\"");
    }
};

class NoCommand : public Command {
public:
    void execute() override {}

    ~NoCommand() override = default;
};


// ---- Commands ----

class Setup : public Command {
public:
    ~Setup() override = default;

    void execute() override {
        bool isSetup = false;
        if (!isSetup) {
            log(INFO, "Setup Started");
            FlowerGroup::setup();

            //TODO: Add function for thread
            FlowerGroup::setThreshold(1, STALL_THRESHOLD, BOUNDARY_OFFSET, STALL_DETECTION_MOVE_BLOCK);
            FlowerGroup::setThreshold(2, STALL_THRESHOLD, BOUNDARY_OFFSET, STALL_DETECTION_MOVE_BLOCK);
        }
        log(RESPONSE, "Setup Finished");

    }

private:
    const int STALL_THRESHOLD = 5;
    const int BOUNDARY_OFFSET = 100;
    const int STALL_DETECTION_MOVE_BLOCK = 100;
};

class Stat : public Command {
public:
    ~Stat() override = default;

    void execute() override {
        FlowerGroup::stat();
    }
};

class NewStepper : public Command {
public:
    int port1;
    int port2;
    int sensor_pin;

    NewStepper(int port1, int port2, int sensor_pin) {
        this->port1 = port1;
        this->port2 = port2;
        this->sensor_pin = sensor_pin;
    }

    ~NewStepper() override = default;

    void execute() override {
        if (FlowerGroup::addStepper(new Flower(port1, port2, sensor_pin))) {
            FlowerGroup::stat();
            log(DEBUG, "Flower " + String(port1) + " " + String(port2) + " " + String(sensor_pin) + " Created.");
            log(DEBUG, "Flower added to the group. Total number of flowers:" + String(FlowerGroup::numFlower()));
        } else {
            log(ERROR, "Failed to add flower " + String(port1) + " " + String(port2) + " " + String(sensor_pin));
        }
    }
};

class Home : public Command {
public:
    explicit Home(int *ids) {
        this->ids = ids;
    }

    ~Home() override {
        delete ids;
    };

    void execute() override {
        FlowerGroup::home(ids);
    }

private:
    int *ids;
};

class Open : public Command {
public:
    Open(int range, int *ids) {
        this->range = range;
        this->ids = ids;
    }

    ~Open() override = default;

    void execute() override {
        if (isAll(ids)) FlowerGroup::move(range);
        else FlowerGroup::move(range, ids);
    }

private:
    int *ids;
    int range;
};

class OpenInd : public Command {
public:
    OpenInd(int *target, int *motors) {
        this->target = target;
        this->motors = motors;
    }

    ~OpenInd() override = default;

    void execute() override {
        FlowerGroup::moveInd(target, motors);
    }

private:
    int *target;
    int *motors;
};

class SoftStop : public Command {
public:
    ~SoftStop() override = default;

    void execute() override {
        FlowerGroup::softStop();
    }
};

class ForceStop : public Command {
public:
    ~ForceStop() override = default;

    void execute() override {
        FlowerGroup::forceStop();
    }
};

class ToggleDebug : public Command {
public:
    ~ToggleDebug() override = default;

    void execute() override {
        bool debug = false;
        log(INFO, "Debug Mode :" + String(debug));
    }
};

class Reset : public Command {
public:
    ~Reset() override = default;

    void execute() override {
        FlowerGroup::reset();
    }
};

class Threshold : public Command {
public:
    Threshold(int id, int stall_threshold, int boundary_offset, int stall_detection_move_block) {
        this->id = id;
        this->stall_threshold = stall_threshold;
        this->boundary_offset = boundary_offset;
        this->stall_detection_move_block = stall_detection_move_block;
    }

    ~Threshold() override = default;

    void execute() override {
        FlowerGroup::setThreshold(id, stall_threshold, boundary_offset, stall_detection_move_block);
    }

private:
    int id;
    int stall_threshold;
    int boundary_offset;
    int stall_detection_move_block;
};

class Speed : public Command {
public:
    explicit Speed(int speed) {
        this->speed = speed;
    }

    ~Speed() override = default;

    void execute() override {
        FlowerGroup::setSpeed(speed);
    }

private:
    int speed;
};

class Acceleration : public Command {
public:
    explicit Acceleration(int acceleration) {
        this->acceleration = acceleration;
    }

    ~Acceleration() override = default;

    void execute() override {
        FlowerGroup::setAcceleration(acceleration);
    }

private:
    int acceleration;
};

class Ping : public Command {
public:
    ~Ping() override = default;

    void execute() override {
        log(RESPONSE, "Ping");
    }
};

class Restart : public Command {
// Cpu Restart for Teensy 3.x and other ARM Cortex-M processors
#define CPU_RESTART_ADDR (*(volatile uint32_t *)0xE000ED0C) // Application Interrupt and Reset Control location
#define CPU_RESTART_VAL 0x5FA0004
public:
    ~Restart() override = default;

    void execute() override {
        Serial.end();  // Clear the serial monitor
        CPU_RESTART_ADDR = CPU_RESTART_VAL;  // Set the hardware restart value
    }
};

#endif // COMMANDS_COMMAND_H

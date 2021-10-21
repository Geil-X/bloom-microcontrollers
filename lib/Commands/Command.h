#ifndef COMMANDS_COMMAND_H
#define COMMANDS_COMMAND_H

#include <Flower.h>

// ---- Abstract Base Class ----

namespace Command {
    enum Id : unsigned char {
        NO_COMMAND,
        SETUP,
        HOME,
        OPEN,
        CLOSE,
        OPEN_TO,
        SPEED,
        ACCELERATION,
    };

    void setup(Flower &flower);

    void home(Flower &flower);

    void open(Flower &flower);

    void close(Flower &flower);

    void openTo(Flower &flower, float percentage);

    void speed(Flower &flower, int speed);

    void acceleration(Flower &flower, int acceleration);
}

#endif  // COMMANDS_COMMAND_H

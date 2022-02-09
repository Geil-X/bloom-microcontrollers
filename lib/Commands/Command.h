#ifndef COMMANDS_COMMAND_H
#define COMMANDS_COMMAND_H

#include <Flower.h>
#include <FastLED.h>

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

    void openTo(Flower &flower, fract16 percentage);

    void speed(Flower &flower, uint16_t speed);

    void acceleration(Flower &flower, uint16_t acceleration);
}

#endif  // COMMANDS_COMMAND_H

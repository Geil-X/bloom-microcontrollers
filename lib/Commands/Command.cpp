#include <Command.h>
#include "Logging.h"

void Command::setup(Flower &flower) {
    flower.setup();
}

void Command::home(Flower &flower) {
    flower.home();
}

void Command::open(Flower &flower) {
    flower.openAsync();
}

void Command::close(Flower &flower) {
    flower.closeAsync();
}

void Command::openTo(Flower &flower, fract16 percentage) {
    flower.openToAsync(percentage);
}

void Command::speed(Flower &flower, uint16_t speed) {
    flower.setMaxSpeed(speed);
}

void Command::acceleration(Flower &flower, uint16_t acceleration) {
    flower.setAcceleration(acceleration);
}

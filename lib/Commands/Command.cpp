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

void Command::openTo(Flower &flower, float percentage) {
    flower.openToAsync((float) percentage);
}

void Command::speed(Flower &flower, int speed) {
    flower.setMaxSpeed((float) speed);
}

void Command::acceleration(Flower &flower, int acceleration) {
    flower.setAcceleration((float) acceleration);
}

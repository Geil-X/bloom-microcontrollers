#include <Arduino.h>
#include <Flower.h>

Flower *flower;

void setup() {
    // Initialization
    flower = new Flower(2, 3, 21);

    // Setup
    flower->setup();
    flower->home();

    // Set parameters
    flower->setSpeed(3000);
    flower->setAcceleration(750);

    // Movement sequence
    flower->open();
    flower->close();
    flower->open(50);
    flower->close();
    flower->open();
    flower->open(50);
    flower->open();
    flower->close();
}

void loop() {

}
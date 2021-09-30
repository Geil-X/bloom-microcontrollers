#ifndef FLOWER_FLOWER_H
#define FLOWER_FLOWER_H

#include <AccelStepper.h>
#include <TMCStepper.h>

#define DEFAULT_MAX_SPEED 200.
#define DEFAULT_SPEED 200.
#define DEFAULT_ACCELERATION 100.

constexpr uint32_t steps_per_mm = 80;

class Flower {
public:
    Flower(uint8_t en, uint8_t dir, uint8_t step, uint8_t cs, uint8_t mosi, uint8_t miso, uint8_t sck, float rSense);

    // Main Functions
    void setup();
    void run();

    // Modifier Functions
    void setSpeed(float speed);
    void setMaxSpeed(float speed);
    void setAcceleration(float acceleration);


private:
    TMC2130Stepper driver;
    AccelStepper stepper;

    uint8_t en;
    uint8_t dir;
    uint8_t step;
    uint8_t cs;
    uint8_t mosi;
    uint8_t miso;
    uint8_t sck;
    float rSense;
};


#endif //FLOWER_FLOWER_H

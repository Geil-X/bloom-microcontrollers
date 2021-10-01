#ifndef FLOWER_FLOWER_H
#define FLOWER_FLOWER_H

#include <AccelStepper.h>
#include <TMCStepper.h>

constexpr uint32_t steps_per_mm = 80;

class Flower {
public:
    Flower(uint8_t en, uint8_t dir, uint8_t step, uint8_t cs, uint8_t mosi, uint8_t miso, uint8_t sck, uint8_t diag1, float rSense);


    // Main Functions

    void setup();


    // Accessor Functions

    /** Get the number of steps between the target position and the current position */
    long remainingDistance();


    // Modifier Functions

    void setMaxSpeed(float speed);
    void setAcceleration(float acceleration);


    // Actions

    /* Run the motor to implement speed and acceleration in order to proceed to the target position
     * You must call this at least once per step, preferably in your main loop
     * If the motor is in the desired position, the cost is very small
     * @returns true if the motor is still running to the target position.
     */
     bool run();

    /** Move the stepper a particular number of millimeters */
    void move(int mm);

    /** Reverse the motor direction */
    void reverse();


    // Stall Detection

    /** Interrupt service routine for telling the flower object that the motor stalled */
    static void motorStalled();

    volatile static bool stalled;

private:
    void setup_driver();
    void setup_stepper();

    // Driver objects
    TMC2130Stepper driver;
    AccelStepper stepper;

    // Pins
    uint8_t enable;
    uint8_t chip_select;
    uint8_t diag1;
};


#endif //FLOWER_FLOWER_H

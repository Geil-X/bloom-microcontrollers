#ifndef FLOWER_FLOWER_H
#define FLOWER_FLOWER_H

#include <AccelStepper.h>
#include <TMCStepper.h>

enum Direction {
    OPEN,
    CLOSE
};

class Flower {
public:
    Flower(uint8_t en, uint8_t dir, uint8_t step, uint8_t cs, uint8_t mosi, uint8_t miso, uint8_t sck, uint8_t diag1,
           float rSense);


    // Main Functions

    /** Initialize the flower hardware. */
    void setup();

    /**
     * Perform the homing sequence for the flower. This lets the flower know
     * where it's boundaries of movement are.
     *
     * Note: this function is blocking.
     */
    void home();

    // Accessor Functions

    /** Get the number of steps between the target position and the current position */
    long remainingDistance();


    // Modifier Functions

    void setMaxSpeed(float speed);
    void setSpeed(float speed);
    void setAcceleration(float acceleration);
    void setDirection(Direction direction);

    // Actions

    void open();
    void close();
    void openTo(float percentage);

    /**
     * Run the motor to implement speed and acceleration in order to proceed to the target position
     * You must call this at least once per step, preferably in your main loop
     * If the motor is in the desired position, the cost is very small
     * @returns true if the motor is still running to the target position.
     */
    bool run();

    /** Move the stepper a particular number of millimeters */
    void move(int steps, Direction direction);
    void move(int steps);

    void moveBlocking(int steps, Direction direction);
    void moveBlocking(int steps);

    /** Reverse the motor direction */
    void reverse();

    void stop();


    // Stall Detection

    /** Interrupt service routine for telling the flower object that the motor stalled */
    static void onStall();

    /**
     * Check if the flower motor has stalled. This is a read and clear
     * operation, so if the motor stalled, the stall value is set to
     * false.
     * */
    static bool motorStalled();

    volatile static bool stalled;

    // Driver objects
    TMC2130Stepper driver;
    AccelStepper stepper;

private:
    void setupDriver();
    void setupStepper();

    int moveUntilStall(Direction direction);

    // Pins
    uint8_t enable;
    uint8_t chip_select;
    uint8_t diag1;

    // Motor positioning
    int current_position;
    int max_steps;
};


#endif //FLOWER_FLOWER_H

#ifndef FLOWER_FLOWER_H
#define FLOWER_FLOWER_H

#include <AccelStepper.h>
#include <TMCStepper.h>


// Toggle between the two modes of operation
// If both are enabled, OPEN_CLOCKWISE wins
//#define OPEN_CLOCKWISE
#define OPEN_COUNTERCLOCKWISE

#define MICROSTEPS 16

// The number of consecutive stalls needed to consider the motor to have stalled
#define CONSECUTIVE_STALLS 5

// The windows of time in which stalled are considered back to back
#define STALL_WINDOW 10 // ms

enum Direction {
    DIRECTION_CLOCKWISE,
    DIRECTION_COUNTERCLOCKWISE,
#if defined(OPEN_CLOCKWISE)
    // For clockwise open
    DIRECTION_OPEN = DIRECTION_CLOCKWISE,
    DIRECTION_CLOSE = DIRECTION_COUNTERCLOCKWISE
#elif defined(OPEN_COUNTERCLOCKWISE)
    // For counterclockwise open
    DIRECTION_CLOSE = DIRECTION_CLOCKWISE,
    DIRECTION_OPEN = DIRECTION_COUNTERCLOCKWISE
#endif
};

class Flower {
public:
    Flower(uint8_t en, uint8_t dir, uint8_t step, uint8_t cs,
           uint8_t mosi, uint8_t miso, uint8_t sck, uint8_t diag1);


    // Main Functions

    /** Initialize the flower hardware. */
    void setup();

    /**
     * Perform the homing sequence for the flower. This lets the flower know
     * where it's boundaries of movement are.
     *
     * @note This function is blocking and resets the max-speed and acceleration
     *       that were previously set. You will need to reset them after calling
     *       this function.
     */
    void home();

    /**
     * Find the boundary of the device for resetting the zero position.  This
     * function is intended to be used after the initial home has already been
     * performed but step was skipped or a stall was detected. This routine will
     * reset the zero position and keep the old step range from the original home
     * sequence.
     *
     * @note This function is blocking.
     */
//    void rehome();


    // Accessors

    /** Get the number of steps between the target position and the current position */
    long remainingDistance();


    // Modifier Functions

    /**
     * Set the max speed of the device.
     * @param speed Maximum speed of the stepper motor is steps per second
     */
    void setMaxSpeed(float speed);

    /** Set the acceleration of the device.
     * @param acceleration Acceleration of the stepper motor is steps per second^2
     */
    void setAcceleration(float acceleration);

    /**
     * Be sure to set this value after the max speed and acceleration.
     * This function won't work otherwise.
     * @param speed Speed of the stepper motor is steps per second
     *
     * @seeitem runSpeed
     */
    void setSpeed(float speed);


    // Actions

    void open();
    void close();
    void openTo(float percentage);

    void openAsync();
    void closeAsync();
    void openToAsync(float percentage);

    /**
     * Run the motor to implement speed and acceleration in order to proceed to the target position
     * You must call this at least once per step, preferably in your main loop
     * If the motor is in the desired position, the cost is very small
     * @returns true if the motor performed a step during this function call.
     *
     * @seeitem openAsync
     * @seeitem closeAsync
     * @seeitem openToAsync
     * @seeitem runSpeed
     */
    bool run();

    /**
     * @returns true if the motor performed a step during this function call.
     *
     * @seeitem openAsync
     * @seeitem closeAsync
     * @seeitem openToAsync
     * @seeitem run
     */
    bool runSpeed();

    /**
     * Check if the flower motor has stalled. This is a read and clear
     * operation, so if the motor stalled, the stall value is set to
     * false.
     * */
    bool motorStalled();

private:
    // Setup Functions

    /** Setup all the configuration for the AccelStepper @see driver */
    void setupDriver();

    /** Setup all the configuration for the TMC2130Stepper @see stepper */
    void setupStepper();


    // Modifiers

    /**
     * Set
     * @param direction The direction of rotation to set the motor.
     */
    void setDirection(Direction direction);

    /**
     * Sets the zero position for the device based off of the current position.
     * This function should be called when the device is in the closed position.
     */
    void setZeroPosition();

    // Actions

    /**
     * Move the motor in a particular direction until the motor stalls out.
     *
     * @param direction The direction to move the motor into
     * @return The number of steps that the motor performed before stalling
     */
    int moveUntilStall(Direction direction);

    /** Move the stepper a particular number of millimeters */
    void moveTo(int target);
    void moveToBlocking(int position);

    /** Move the stepper */
    void move(int steps, Direction direction);

    void moveBlocking(int steps, Direction direction);

    /** Reverse the motor direction */
    void reverse();

    /** Clear all motor actions */
    void stop();


    // Stall Detection

    /** Interrupt service routine for telling the flower object that the motor stalled */
    static void onStall();
    static void clearStalls();

    /** Variable that stores the stall state of the flower. */
    volatile static int stall_count;

    // Driver objects
    TMC2130Stepper driver;
    AccelStepper stepper;

    // Pins
    uint8_t enable;
    uint8_t direction;
    uint8_t step;
    uint8_t chip_select;
    uint8_t diag1;

    // Motor positioning
    int max_steps;
};


#endif //FLOWER_FLOWER_H

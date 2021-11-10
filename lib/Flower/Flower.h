#ifndef FLOWER_FLOWER_H
#define FLOWER_FLOWER_H

#include <AccelStepper.h>
#include <TMCStepper.h>
#include <VoltageSensor.h>
#include <StallDetectionTuning.h>

// Toggle between the two modes of operation
// If both are enabled, OPEN_CLOCKWISE wins
//#define OPEN_CLOCKWISE
#define OPEN_COUNTERCLOCKWISE

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
           uint8_t mosi, uint8_t miso, uint8_t sck, uint8_t diag1,
           uint8_t vm);


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


    // Accessor Functions

    int getStallGuardThreshold() const;
    unsigned int getStallDetectionThreshold() const;
    unsigned long getSpeed();
    unsigned int getStallGuardResult() const;

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
     * Set
     * @param direction The direction of rotation to set the motor.
     */
    void setDirection(Direction direction);

    /**
     * Be sure to set this value after the max speed and acceleration.
     * This function won't work otherwise.
     * @param speed Speed of the stepper motor is steps per second
     *
     * @seeitem runSpeed
     */
    void setSpeed(float speed);

    void setStallGuardThreshold(int sgt);
    void setStallDetectionThreshold(int threshold);

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

    // Accessors

    /**
     * Check if the flower motor has stalled. This is a read and clear
     * operation, so if the motor stalled, the stall value is set to
     * false.
     * */
    bool motorStalled();

    /**
     * Check if the motor has regained power. This is a read and clear
     * operation.
     *
     * @note This function should be called fairly often so that it can monitor
     *       for power outages and check for moments when power is regained.
     *
     * @return If the motor has regained power.
     */
    bool regainedPower();

    bool hasPower();

    /** Reverse the motor direction */
    void reverse();

private:
    // Setup Functions

    /** Setup all the configuration for the AccelStepper @see driver */
    void setupDriver();

    /** Setup all the configuration for the TMC2130Stepper @see stepper */
    void setupStepper();


    // Modifiers

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

    /** Clear all motor actions */
    void stop();


    // Stall Detection

    /**
     * Update the stall guard parameters based on the current running speed.
     * The motor driver stall guard threshold and the maximum detection
     * threshold need to be adjusted because they are speed dependent.
     */
    void updateStallParameters();

    /** Variable that stores the stall state of the flower. */
    unsigned long stall_read_time = 0;
    unsigned long last_stall = 0;

    /** When the stall guard result drops below this value, a stall is detected. [1, 1022] */
    unsigned int stall_detection_threshold = 800;
    int stall_guard_threshold = 63;
    unsigned int stall_guard_result = 1023;

    /** Voltage Detection */
    bool lost_power = false;

    // Driver objects
    TMC2130Stepper driver;
    AccelStepper stepper;

    // Sensors
    VoltageSensor motor_voltage;

    // Pins
    uint8_t enable;
    uint8_t direction;
    uint8_t step;
    uint8_t chip_select;
    uint8_t diag1;

    // Motor positioning
    int max_steps = 0;
};


#endif //FLOWER_FLOWER_H

#ifndef FLOWER_FLOWER_H
#define FLOWER_FLOWER_H

#include <AccelStepper.h>
#include <TMCStepper.h>
#include <VoltageSensor.h>
#include <StallDetectionTuning.h>
#include <FastLED.h>

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
           uint8_t mosi, uint8_t miso, uint8_t sck);


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

    /**
     * @return The fractional position of the flower between 0 and 1
     */
    [[nodiscard]] fract16 getPosition();
    [[nodiscard]] int8_t getStallGuardThreshold();
    [[nodiscard]] uint16_t getStallDetectionThreshold() const;
    [[maybe_unused]] [[nodiscard]] uint16_t getSpeed();
    [[nodiscard]] uint16_t getStallGuardResult() const;
    [[nodiscard]] bool isOpen();
    [[nodiscard]] bool isClosed();
    [[nodiscard]] bool isAtTarget();

    // Modifier Functions

    /**
     * Set the max speed of the device.
     * @param speed Maximum speed of the stepper motor is steps per second
     */
    void setMaxSpeed(uint16_t speed);

    /** Set the acceleration of the device.
     * @param acceleration Acceleration of the stepper motor is steps per second^2
     */
    void setAcceleration(uint16_t acceleration);

    /**
     * Set
     * @param dir The direction of rotation to set the motor.
     */
    void setDirection(Direction dir);

    /**
     * Be sure to set this value after the max speed and acceleration.
     * This function won't work otherwise.
     * @param speed Speed of the stepper motor is steps per second
     *
     * @seeitem runSpeed
     */
    void setSpeed(uint16_t speed);

    void setStallGuardThreshold(int8_t sgt);
    void setStallDetectionThreshold(uint16_t threshold);

    // Actions

    void open();
    void close();
    void openTo(fract16 percentage);

    void openAsync();
    void closeAsync();
    void openToAsync(fract16 percentage);

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
     * Update the stall guard parameters based on the current running speed.
     * The motor driver stall guard threshold and the maximum detection
     * threshold need to be adjusted because they are speed dependent.
     */
    void updateStallParameters();

    // Accessors

    /**
     * Check if the flower motor has stalled. This is a read and clear
     * operation, so if the motor stalled, the stall value is set to
     * false.
     * */
    bool motorStalled();

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
     * @param dir The direction to move the motor into
     * @return The number of steps that the motor performed before stalling
     */
    uint16_t moveUntilStall(Direction dir);

    /** Move the stepper a particular number of millimeters */
    void moveTo(uint16_t target);
    void moveToBlocking(uint16_t position);

    /** Move the stepper */
    void move(uint16_t steps);

    void moveBlocking(uint16_t steps);

    /** Clear all motor actions */
    [[maybe_unused]] void stop();


    // Stall Detection

    /** Variable that stores the stall state of the flower. */
    uint32_t stall_read_time = 0;
    uint32_t last_stall = 0;

    /**
     * When the stall guard result drops below this value, a stall is detected.
     * This value is an unsigned 10 bit number in the range [1, 1022].
     * */
    uint16_t stall_detection_threshold = 800;

    /**
     * The last stall guard reading.
     * This value is a signed 7 bit number in the range [-64, 63].
     */
    uint16_t stall_guard_result = 1023;


    // Motor positioning
    uint16_t max_steps = 0;

    // Driver objects
    TMC2130Stepper driver;
    AccelStepper stepper;

    // Pins
    uint8_t enable;
    uint8_t direction;
    uint8_t step;
    uint8_t chip_select;
};

#endif //FLOWER_FLOWER_H
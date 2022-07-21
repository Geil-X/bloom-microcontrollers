#ifndef FLOWER_FLOWER_H
#define FLOWER_FLOWER_H

#include <AccelStepper.h>
#include <TMCStepper.h>
#include <FastLED.h>

#include <Types.h>
#include <StallDetectionTuning.h>

class Flower {
public:
    Flower(PinIndex en, PinIndex dir, PinIndex step, PinIndex cs,
           PinIndex mosi, PinIndex miso, PinIndex sck);


// ---- Main Functions --------------------------------------------------------


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


// ---- Accessor Functions -----------------------------------------------------

    [[nodiscard]] Position getPosition();
    [[nodiscard]] Position getTarget();
    [[nodiscard]] Speed getSpeed();
    [[nodiscard]] Speed getMaxSpeed();
    [[nodiscard]] Acceleration getAcceleration() const;
    [[nodiscard]] StallTuning getStallGuardThreshold();
    [[nodiscard]] StallLevel getStallDetectionThreshold() const;
    [[nodiscard]] StallLevel getStallGuardResult() const;
    [[nodiscard]] bool isOpen();
    [[nodiscard]] bool isClosed();
    [[nodiscard]] bool isAtTarget();
    /**
     * Check if the flower motor has stalled. This is a read and clear
     * operation, so if the motor stalled, the stall value is set to
     * false.
     * */
    bool motorStalled();

    /** Reverse the motor direction */
    void reverse();

// ---- Modifier Functions ----------------------------------------------------

    /**
     * Set the max speed of the device.
     * @param speed Maximum speed of the stepper motor is steps per second
     */
    void setMaxSpeed(Speed speed);

    /** Set the acceleration of the device.
     * @param acceleration Acceleration of the stepper motor is steps per second^2
     */
    void setAcceleration(Acceleration acceleration);

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

    void setStallGuardThreshold(StallTuning sgt);
    void setStallDetectionThreshold(StallLevel threshold);

    // Actions

    void open();
    __attribute__((unused)) void close();
    void openTo(Percentage percentage);

    void openAsync();
    void closeAsync();
    void openToAsync(Percentage percentage);

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
    Steps moveUntilStall(Direction dir);

    /** Move the stepper a particular number of millimeters */
    void moveTo(Position target);
    void moveToBlocking(Position position);

    /** Move the stepper */
    void move(Steps steps);

    void moveBlocking(Steps steps);

    /** Clear all motor actions */
    [[maybe_unused]] void stop();


    // Stall Detection

    /** Variable that stores the stall state of the flower. */
    Time stall_read_time = 0;
    Time last_stall = 0;

    /**
     * When the stall guard result drops below this value, a stall is detected.
     * This value is an unsigned 10 bit number in the range [1, 1022].
     * */
    StallLevel stall_detection_threshold = 800;

    /**
     * The last stall guard reading.
     * This value is a signed 7 bit number in the range [-64, 63].
     */
    StallLevel stall_guard_result = 1023;


    // Motor Properties
    Steps max_steps = 0;
    Acceleration acceleration = 0;

    // Driver objects
    TMC2130Stepper driver;
    AccelStepper stepper;

    // Pins
    PinIndex enable;
    PinIndex direction;
    PinIndex step;
    PinIndex chip_select;
};

#endif //FLOWER_FLOWER_H
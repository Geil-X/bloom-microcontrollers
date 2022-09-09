#include <Flower.h>

#include <FastLED.h>
#include <MathExtra.h>
#include <Logging.h>

using namespace TMC2130_n;

// Set stepper driver microsteps [1-255]
#define MICROSTEPS 16

#define STALL_BUFFER 150 // ms
#define STALL_CHECK_TIME 5 // ms

// TMC2130 Parameters
#define R_SENSE 0.11f  // Set for the silent step stick series


enum SPREAD_CYCLE {
    SPREAD_CYCLE_ENABLED [[maybe_unused]] = 0,
    SPREAD_CYCLE_DISABLED = 1
};


Flower::Flower(
        PinIndex en, PinIndex dir, PinIndex step, PinIndex cs,
        PinIndex mosi, PinIndex miso, PinIndex sck)
        : driver(cs, R_SENSE, mosi, miso, sck),
          stepper(stepper.DRIVER, step, dir) {

    // Pin initialization
    this->enable = en;
    this->direction = dir;
    this->step = step;
    this->chip_select = cs;
    this->stall_read_time = millis();
}

// ---- Main Functions ----

void Flower::setup() {
    setupDriver();
    setupStepper();
}

void Flower::setupDriver() {
    // Enable the chip select pin
    pinMode(chip_select, OUTPUT);
    pinMode(enable, OUTPUT);
    pinMode(direction, OUTPUT);
    pinMode(step, OUTPUT);

    digitalWrite(chip_select, HIGH);
    digitalWrite(enable, HIGH);  // Deactivate driver (Enable Low)
    digitalWrite(direction, LOW);  //LOW or HIGH
    digitalWrite(step, LOW);

    pinMode(MISO, INPUT_PULLUP);

    // Initiate pins and registers
    driver.begin();

    // Off time setting controls duration of slow decay phase
    // N_CLK= 12 + 32*TOFF
    driver.toff(4);  // us

    // Set comparator blank time to 16, 24, 36 or 54 clocks
    // Range: %00 … %11:
    // Hint: %01 or %10 is recommended for most applications
    driver.tbl(1);

    // Set the max hardware current
    driver.rms_current(300);  // mA

    // Set driver microsteps, [1-255]
    driver.microsteps(MICROSTEPS);

    // Enable stealth chop, extremely quiet stepping
    driver.en_pwm_mode(true);

    // Enable automatic current control
    // Attention: When using a user defined sine wave table, the amplitude of this sine wave table should
    // not be less than 244. Best results are obtained with 247 to 252 as peak values.
    driver.pwm_autoscale(true);

    // User defined amplitude
    // pwm_autoscale = 0
    //     User defined PWM amplitude offset (0-255)
    //     The resulting amplitude (limited to 0…255)
    //     is: PWM_AMPL + PWM_GRAD * 256 / TSTEP
    // pwm_autoscale = 1
    //     User defined maximum PWM amplitude when switching back from current chopper
    //     mode to voltage PWM mode (switch over velocity defined by TPWMTHRS). Do not set
    //     too low values, as the regulation cannot measure the current when the actual PWM
    //     value goes below a setting specific value. Settings above 0x40 recommended.
    driver.pwm_ampl(255);

    //  chm=0 %000 … %111:
    //     Add 1, 2, …, 8 to hysteresis low value HEND (1/512 of this setting adds to current setting)
    //     Attention: Effective HEND+HSTRT ≤ 16.
    //     Hint: Hysteresis decrement is done each 16 clocks
    //  chm=1 %0000 … %1111:
    //     Fast decay time setting (MSB: fd3):
    //     Fast decay time setting TFD with NCLK= 32*TFD (%0000: slow decay only)
    driver.hysteresis_start(4);

    // chm=1 %0000 … %1111:
    //     Offset is -3, -2, -1, 0, 1, …, 12
    //     This is the sine wave offset and 1/512 of the value becomes added to the absolute value
    //     of each sine wave entry.
    // chm=0 %0000 … %1111:
    //     Hysteresis is -3, -2, -1, 0, 1, …, 12 (1/512 of this setting adds to current setting)
    //     This is the hysteresis value which becomes used for the hysteresis chopper.
    driver.hysteresis_end(0);

    // Set the chopper mode
    // - SPREAD_CYCLE_ENABLED = 0:
    //     Standard mode (spreadCycle)
    // - SPREAD_CYCLE_DISABLED = 1:
    //     Constant off time with fast decay time.
    //     Fast decay time is also terminated when the
    //     negative nominal current is reached. Fast decay is
    //     after on time.
    driver.chm(SPREAD_CYCLE_DISABLED);

    // 20bit max. Lower velocity threshold for switching on smart energy coolStep and stall guard
    // Needs to be enabled before activating DIAG1_PIN on motor stall
    driver.TCOOLTHRS(0xFFFFF);

    // This velocity setting allows velocity dependent switching into a different chopper mode and
    // full stepping to maximize torque. (unsigned)
    // The stall detection feature becomes switched off for 2-3 electrical periods whenever passing
    // THIGH threshold to compensate for the effect of switching modes.
    driver.THIGH(0);

    // Minimum stallGuard2 value for smart current control and smart current enable
    // If the stallGuard2 result falls below SEMIN*32,
    // the motor current becomes increased to reduce motor load angle.
    // 0 (Default): smart current control coolStep off
    driver.semin(5);

    // stallGuard2 hysteresis value for smart current control
    // If the stallGuard2 result is equal to or above (SEMIN+SEMAX+1)*32,
    // the motor current becomes decreased to save energy.
    driver.semax(2);

    // Current down step speed
    // 0b00: For each 32 stallGuard2 values decrease by one
    // 0b01: For each 8 stallGuard2 values decrease by one
    // 0b10: For each 2 stallGuard2 values decrease by one
    // 0b11: For each stallGuard2 value decrease by one
    driver.sedn(0b01);

    // This signed value controls stallGuard2 level for stall output and sets the optimum
    // measurement range for readout. A lower value gives a higher sensitivity and will
    // give more stall detections while a lower value will be less likely to give a
    // stall detection.
    // Stall guard threshold range -64 to +63:
    driver.sgt(30);
}

void Flower::setupStepper() {
    stepper.setEnablePin(enable);
    stepper.setPinsInverted(false, false, true);
    stepper.enableOutputs();
}

/**
 * A delay is required between @see moveUntilStall calls so that the stall from
 * the previous motor movement isn't carried forward to the new movement call.
 *
 * The last call to @see moveUntilStall with @see OPEN is to get the motor in
 * the fully open position to get the max number of steps that the motor can
 * move within.
 */
void Flower::home() {
    int delay_ms = 200;

    // Set homing routine parameters
    setMaxSpeed(15000);
    setAcceleration(5000);
    setSpeed(5000);
    setStallGuardThreshold(40);
    setStallDetectionThreshold(950);

    // Fully open the motor
    delay(delay_ms);
    moveUntilStall(DIRECTION_OPEN);

    // Fully close the motor and get the number of steps for a full swing
    delay(delay_ms);
    max_steps = moveUntilStall(DIRECTION_CLOSE);

    // Apply a boundary buffer
    int buffer_steps = (int) (max_steps * 0.1);
    moveBlocking(buffer_steps / 2);
    max_steps -= buffer_steps * 2;

    Log::debug("Homing total steps = " + String(max_steps));

    // Set the zero position for the device
    setZeroPosition();
    moveTo((int) stepper.currentPosition());
}

// ---- Accessor Functions ----

Position Flower::getPosition() {
#if defined(OPEN_CLOCKWISE)
    return map(stepper.currentPosition(), 0, max_steps, 0, UINT16_MAX);
#elif defined(OPEN_COUNTERCLOCKWISE)
    return map(max_steps - stepper.currentPosition(), 0, max_steps, 0, UINT16_MAX);
#endif
}

Position Flower::getTarget() {
#if defined(OPEN_CLOCKWISE)
    return getPosition() + stepper.distanceToGo();
#elif defined(OPEN_COUNTERCLOCKWISE)
    return getPosition() - stepper.distanceToGo();
#endif
}

StallTuning Flower::getStallGuardThreshold() {
    return driver.sgt();
}

StallLevel Flower::getStallDetectionThreshold() const {
    return stall_detection_threshold;
}

Acceleration Flower::getSpeed() {
    return (uint16_t) stepper.speed();
}

Acceleration Flower::getMaxSpeed() {
    return (uint16_t) stepper.maxSpeed();
}

Acceleration Flower::getAcceleration() const {
    return acceleration;
}

StallLevel Flower::getStallGuardResult() const {
    return stall_guard_result;
}

bool Flower::isOpen() {
#if defined(OPEN_CLOCKWISE)
    return stepper.currentPosition() == max_steps;
#elif defined(OPEN_COUNTERCLOCKWISE)
    return stepper.currentPosition() == 0;
#endif
}

bool Flower::isClosed() {
#if defined(OPEN_CLOCKWISE)
    return stepper.currentPosition() == 0;
#elif defined(OPEN_COUNTERCLOCKWISE)
    return stepper.currentPosition() == max_steps;
#endif
}

bool Flower::isAtTarget() {
    return stepper.distanceToGo() == 0;
}

// ---- Modifier Functions ----

void Flower::setMaxSpeed(uint16_t speed) {
    stepper.setMaxSpeed(speed);
}

void Flower::setSpeed(uint16_t speed) {
    stepper.setSpeed(speed);
}

void Flower::setAcceleration(Acceleration newAcceleration) {
    this->acceleration = newAcceleration;
    stepper.setAcceleration(acceleration);
}

void Flower::setDirection(Direction dir) {
    switch (dir) {
        case DIRECTION_CLOCKWISE:
            driver.shaft(false);
            break;
        case DIRECTION_COUNTERCLOCKWISE:
            driver.shaft(true);
            break;
    }
}

void Flower::setStallGuardThreshold(int8_t sgt) {
    driver.sgt(sgt);
}

void Flower::setStallDetectionThreshold(uint16_t threshold) {
    stall_detection_threshold = threshold;
}

void Flower::setZeroPosition() {
#if defined(OPEN_CLOCKWISE)
    stepper.setCurrentPosition(0);
#elif defined(OPEN_COUNTERCLOCKWISE)
    stepper.setCurrentPosition(max_steps);
#endif
}

void Flower::open() {
    openTo(UINT16_MAX);
}

__attribute__((unused)) void Flower::close() {
    openTo(0);
}

void Flower::openTo(fract16 percentage) {
    uint16_t target;
#if defined(OPEN_CLOCKWISE)
    target = map16(percentage, 0, max_steps);
#elif defined(OPEN_COUNTERCLOCKWISE)
    target = max_steps - map16(percentage, 0, max_steps);
#endif
    moveToBlocking(target);
}

void Flower::openAsync() {
    openToAsync(UINT16_MAX);
}

void Flower::closeAsync() {
    openToAsync(0);
}

void Flower::openToAsync(fract16 percentage) {
    uint16_t target;
    setDirection(DIRECTION_CLOCKWISE);
#if defined(OPEN_CLOCKWISE)
    target = map16(percentage, 0, max_steps);
#elif defined(OPEN_COUNTERCLOCKWISE)
    target = max_steps - map16(percentage, 0, max_steps);
#endif
    moveTo(target);
}

bool Flower::run() {
//    updateStallParameters();
    return stepper.run();
}

bool Flower::runSpeed() {
//    updateStallParameters();
    return stepper.runSpeed();
}

void Flower::move(uint16_t steps) {
    stepper.move(steps);
}

void Flower::moveBlocking(uint16_t steps) {
    move(steps);
    stepper.runToPosition();
}

void Flower::moveTo(uint16_t target) {
    stepper.moveTo(target);
}

void Flower::moveToBlocking(uint16_t position) {
    stepper.runToNewPosition(position);
}

Steps Flower::moveUntilStall(Direction dir) {
    int steps = 0;
    setDirection(dir);

    last_stall = millis();
    while (!motorStalled()) {
        if (runSpeed()) {
            steps++;
        }
    }

    return steps;
}

void Flower::reverse() {
    driver.shaft(!driver.shaft());
}

[[maybe_unused]] void Flower::stop() {
    stepper.stop();
}

void Flower::updateStallParameters() {
    float current_speed = stepper.speed();

    if (current_speed < (float) STALL_TABLE[0][SPEED_COLUMN]) {
        driver.sgt((int8_t) STALL_TABLE[0][SGT_COLUMN]);
        stall_detection_threshold = STALL_TABLE[0][STALL_DETECTION_COLUMN];
        return;
    }

    for (int i = 1; i < NUM_STALL_VALUES; i++) {
        int table_speed = STALL_TABLE[i][SPEED_COLUMN];

        if ((float) table_speed > current_speed) {
            int lower_speed = STALL_TABLE[i - 1][SPEED_COLUMN];
            int upper_speed = STALL_TABLE[i][SPEED_COLUMN];
            int lower_sgt = STALL_TABLE[i - 1][SGT_COLUMN];
            int upper_sgt = STALL_TABLE[i][SGT_COLUMN];
            int lower_detection_threshold = STALL_TABLE[i - 1][STALL_DETECTION_COLUMN];
            int upper_detection_threshold = STALL_TABLE[i][STALL_DETECTION_COLUMN];

            auto stall_guard_threshold = (int8_t) map(
                    (int) current_speed,
                    lower_speed, upper_speed,
                    lower_sgt, upper_sgt);
            driver.sgt(stall_guard_threshold);

            stall_detection_threshold = map(
                    (int) current_speed,
                    lower_speed, upper_speed,
                    lower_detection_threshold, upper_detection_threshold);

            return;
        }
    }
}

// ---- Stall Detection ----

bool Flower::motorStalled() {
#define MINIMUM_STALL_SPEED 500
    if (stepper.speed() < MINIMUM_STALL_SPEED) {
        return false;
    }

    unsigned long ms = millis();
    if (ms - stall_read_time > STALL_CHECK_TIME) {
        stall_guard_result = driver.sg_result();
        if (ms - last_stall > STALL_BUFFER && stall_guard_result < stall_detection_threshold) {
            last_stall = ms;
            return true;
        }
        stall_read_time = ms;
    }
    return false;
}

#include "Flower.h"

using namespace TMC2130_n;

// Set stepper driver microsteps [1-255]
#define MICROSTEPS 16

// The stall guard threshold depends on the number of microsteps
#define STALL_GUARD_THRESHOLD 30

#define STALL_WINDOW_MILLIS 50

// The number of consecutive stalls needed to consider the motor to have stalled
#define CONSECUTIVE_STALLS 1

// TMC2130 Parameters
#define R_SENSE 0.11f  // Set for the silent step stick series

// Resistors in the voltage dividers for measuring the motor voltage
#define VOLTAGE_RESISTOR_1 1000  // 1M Ohm
#define VOLTAGE_RESISTOR_2 100   // 100k Ohm


enum SPREAD_CYCLE {
    SPREAD_CYCLE_ENABLED = 0,
    SPREAD_CYCLE_DISABLED = 1
};


Flower::Flower(
        uint8_t en, uint8_t dir, uint8_t step, uint8_t cs,
        uint8_t mosi, uint8_t miso, uint8_t sck, uint8_t diag1,
        uint8_t vm)
        : driver(cs, R_SENSE, mosi, miso, sck),
          stepper(stepper.DRIVER, step, dir),
          motor_voltage(vm, VOLTAGE_RESISTOR_1, VOLTAGE_RESISTOR_2) {

    // Pin initialization
    this->enable = en;
    this->direction = dir;
    this->step = step;
    this->chip_select = cs;
    this->diag1 = diag1;
}

volatile int Flower::stall_count = 0;
volatile unsigned long Flower::stall_time = 0;

// ---- Main Functions ----

void Flower::setup() {
    setupDriver();
    setupStepper();

}

void Flower::setupDriver() {
    // Enable the chip select pin
    pinMode(diag1, INPUT);
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
    driver.rms_current(400);  // mA

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
    // measurement range for readout. A lower value gives a higher sensitivity. Zero is the
    // starting value working with most motors.
    // Stall guard threshold range -64 to +63:
    // A higher value makes stallGuard2 less sensitive and requires more torque to indicate a stall.
    driver.sgt(STALL_GUARD_THRESHOLD);

    // Enable DIAG1_PIN active on motor stall (set TCOOLTHRS before using this feature)
    driver.diag1_stall(true);
    driver.diag1_pushpull(true);  // Active high

    attachInterrupt(digitalPinToInterrupt(diag1), Flower::onStall, RISING);
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
    setMaxSpeed(5000);
    setAcceleration(2000);
    setSpeed(1500);

    clearStalls();

    // Fully open the motor
    delay(delay_ms);
    moveUntilStall(DIRECTION_OPEN);

    // Fully close the motor and get the number of steps for a full swing
    delay(delay_ms);
    max_steps = moveUntilStall(DIRECTION_CLOSE);

    // Apply a boundary buffer
    int buffer_steps = (int) (max_steps * 0.1);
    moveBlocking(buffer_steps / 2, DIRECTION_OPEN);
    max_steps -= buffer_steps * 2;

    // Set the zero position for the device
    setZeroPosition();
    moveTo((int) stepper.currentPosition());
}


// ---- Accessor Functions ----

long Flower::remainingDistance() {
    return stepper.distanceToGo();
}


// ---- Modifier Functions ----

void Flower::setMaxSpeed(float speed) {
    stepper.setMaxSpeed(MICROSTEPS * speed);
}

void Flower::setSpeed(float speed) {
    stepper.setSpeed(MICROSTEPS * speed);
}

void Flower::setAcceleration(float acceleration) {
    stepper.setAcceleration(MICROSTEPS * acceleration);
}

void Flower::setDirection(Direction direction) {
    switch (direction) {
        case DIRECTION_CLOCKWISE:
            driver.shaft(false);
            break;
        case DIRECTION_COUNTERCLOCKWISE:
            driver.shaft(true);
            break;
    }
}

void Flower::setZeroPosition() {
#if defined(OPEN_CLOCKWISE)
    stepper.setCurrentPosition(0);
#elif defined(OPEN_COUNTERCLOCKWISE)
    stepper.setCurrentPosition(max_steps);
#endif
}

void Flower::open() {
    openTo(100);
}

void Flower::close() {
    openTo(0);
}

void Flower::openTo(float percentage) {
    percentage = constrain(percentage, 0, 100);
#if defined(OPEN_CLOCKWISE)
    int target = (int) (percentage * (max_steps / 100.));
#elif defined(OPEN_COUNTERCLOCKWISE)
    int target = max_steps - (int) (percentage * (max_steps / 100.));
#endif
    moveToBlocking(target);
}

void Flower::openAsync() {
    openToAsync(100);
}

void Flower::closeAsync() {
    openToAsync(0);
}

void Flower::openToAsync(float percentage) {
    percentage = constrain(percentage, 0, 100);
    setDirection(DIRECTION_CLOCKWISE);
#if defined(OPEN_CLOCKWISE)
    int target = (int) (percentage * (max_steps / 100.));
#elif defined(OPEN_COUNTERCLOCKWISE)
    int target = max_steps - (int) (percentage * (max_steps / 100.));
#endif

    moveTo(target);
}

bool Flower::run() {
    return stepper.run();
}

bool Flower::runSpeed() {
    return stepper.runSpeed();
}

void Flower::move(int steps, Direction direction) {
    setDirection(direction);
    stepper.move(steps);
}

void Flower::moveBlocking(int steps, Direction direction) {
    move(steps, direction);
    stepper.runToPosition();
}

void Flower::moveTo(int target) {
    stepper.moveTo(target);
}

void Flower::moveToBlocking(int position) {
    stepper.runToNewPosition(position);
}

int Flower::moveUntilStall(Direction direction) {
    int steps = 0;
    setDirection(direction);

    while (!motorStalled()) {
        if (runSpeed()) {
            steps++;
        }
    }

    return steps - CONSECUTIVE_STALLS;
}

void Flower::reverse() {
    driver.shaft(!driver.shaft());
}

void Flower::stop() {
    stepper.stop();
}

// ---- Stall Detection ----

void Flower::onStall() {
    if (stall_count == 0) {
        stall_time = millis();
    } else if (millis() - stall_time > STALL_WINDOW_MILLIS) {
        stall_time = millis();
        stall_count = 0;
    }

    stall_count++;
}

void Flower::clearStalls() {
    stall_count = 0;
}

bool Flower::motorStalled() {
    if (stall_count >= CONSECUTIVE_STALLS) {
        clearStalls();
        return true;
    }

    return false;
}

bool Flower::regainedPower() {
    bool has_power = hasPower();
    if (has_power && lost_power) {
        lost_power = false;
        return true;
    }
    if (!has_power && !lost_power) {
        lost_power = true;
        return false;
    }
    return false;
}

bool Flower::hasPower() {
#define VOLTAGE_THRESHOLD 4.5 // volts
    return motor_voltage.read() >= VOLTAGE_THRESHOLD;
}


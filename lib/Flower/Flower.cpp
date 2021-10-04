#include "Flower.h"

using namespace TMC2130_n;

#define BOUNDARY_STEPS 100

Flower::Flower(uint8_t en, uint8_t dir, uint8_t step, uint8_t cs, uint8_t mosi, uint8_t miso, uint8_t sck,
               uint8_t diag1, float rSense)
        : driver(cs, rSense, mosi, miso, sck), stepper(stepper.DRIVER, step, dir) {

    // Pin initialization
    this->enable = en;
    this->chip_select = cs;
    this->diag1 = diag1;
    this->current_position = 0;
    this->max_steps = 0;
}

volatile bool Flower::stalled = false;

// ---- Main Functions ----

void Flower::setup() {
    setupDriver();
    setupStepper();

    setMaxSpeed(10000);
    setAcceleration(5000);
}

void Flower::setupDriver() {
    // Enable the chip select pin
    pinMode(diag1, INPUT);
    pinMode(chip_select, OUTPUT);
    digitalWrite(chip_select, HIGH);

    // Initiate pins and registers
    driver.begin();

    // Off time setting controls duration of slow decay phase
    // N_CLK= 12 + 32*TOFF
    driver.toff(4);  // us

    // Set the max hardware current
    driver.rms_current(600);  // mA

    // Enable stealth chop, extremely quiet stepping
    driver.en_pwm_mode(HIGH);

    // Enable automatic current control
    // Attention: When using a user defined sine wave table, the amplitude of this sine wave table should
    // not be less than 244. Best results are obtained with 247 to 252 as peak values.
    driver.pwm_autoscale(HIGH);

    // Set driver microsteps, [1-255]
    driver.microsteps(16);

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

    // 20bit max. Lower velocity threshold for switching on smart energy coolStep and stall guard
    // Needs to be enabled before activating DIAG1_PIN on motor stall
    driver.TCOOLTHRS(0xFFFFF);

    // This velocity setting allows velocity dependent switching into a different chopper mode and
    // full stepping to maximize torque. (unsigned)
    // The stall detection feature becomes switched off for 2-3 electrical periods whenever passing
    // THIGH threshold to compensate for the effect of switching modes.
    driver.THIGH(0);

    // This signed value controls stallGuard2 level for stall output and sets the optimum
    // measurement range for readout. A lower value gives a higher sensitivity. Zero is the
    // starting value working with most motors.
    // Stall guard threshold range -64 to +63:
    // A higher value makes stallGuard2 less sensitive and requires more torque to indicate a stall.
    driver.sgt(30);

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

void Flower::home() {
    int delay_ms = 500;
    int steps;

    moveBlocking(BOUNDARY_STEPS, CLOSE);
    moveUntilStall(OPEN);
    delay(delay_ms);
    moveUntilStall(CLOSE);
    delay(delay_ms);
    moveUntilStall(OPEN);
    delay(delay_ms);
    steps = moveUntilStall(CLOSE);
    delay(delay_ms);

    max_steps = steps - BOUNDARY_STEPS * 2;
    moveBlocking(BOUNDARY_STEPS, OPEN);
}

// ---- Accessor Functions ----

long Flower::remainingDistance() {
    return stepper.distanceToGo();
}

// ---- Modifier Functions ----

void Flower::setMaxSpeed(float speed) {
    stepper.setMaxSpeed(speed);
}

void Flower::setSpeed(float speed) {
    stepper.setSpeed(speed);
}

void Flower::setAcceleration(float acceleration) {
    stepper.setAcceleration(acceleration);
}

void Flower::setDirection(Direction direction) {
    switch (direction) {
        case OPEN:
            driver.shaft(true);
            break;
        case CLOSE:
            driver.shaft(false);
            break;
    }
}

// ---- Actions ----

void Flower::open() {
    openTo(100);
}

void Flower::close() {
    openTo(0);
}

void Flower::openTo(float percentage) {
    int target = (int) (percentage * (max_steps / 100.));
    int steps = abs(target - current_position);
    Direction direction = target > current_position ? OPEN : CLOSE;

    moveBlocking(steps, direction);
    current_position = target;
}

bool Flower::run() {
    return stepper.run();
}

void Flower::move(int steps, Direction direction) {
    setDirection(direction);
    stepper.move(steps);
}

void Flower::moveBlocking(int steps, Direction direction) {
    move(steps, direction);
    while (remainingDistance() != 0) {
        run();
    }
}

int Flower::moveUntilStall(Direction direction) {
    int steps = 0;
    move(5000, direction);

    while (!motorStalled()) {
        if (run()) steps++;
    }

    stalled = false;
    return steps;

}

void Flower::reverse() {
    driver.shaft(!driver.shaft());
}

void Flower::stop() {
    stepper.stop();
}

// ---- Stall Detection ----

void Flower::onStall() {
    stalled = true;
}

bool Flower::motorStalled() {
    if (stalled) {
        stalled = false;
        return true;
    }
    return false;
}
#include "Flower.h"

using namespace TMC2130_n;

Flower::Flower(uint8_t en, uint8_t dir, uint8_t step, uint8_t cs, uint8_t mosi, uint8_t miso, uint8_t sck, uint8_t diag1, float rSense)
        : driver(cs, rSense, mosi, miso, sck), stepper(stepper.DRIVER, step, dir) {

    // Pin initialization
    this->enable = en;
    this->chip_select = cs;
    this->diag1 = diag1;
}

volatile bool Flower::stalled = false;

// ---- Main Functions ----

void Flower::setup() {
    setup_driver();
    setup_stepper();

    setMaxSpeed(1000);
    setAcceleration(350);
}

void Flower::setup_driver() {
    // Enable the chip select pin
    pinMode(chip_select, OUTPUT);
    digitalWrite(chip_select, HIGH);

    // Initiate pins and registers
    driver.begin();

    // Off time setting controls duration of slow decay phase
    // N_CLK= 12 + 32*TOFF
    driver.toff(4);  // us

    // Set comparator blank time to 16, 24, 36 or 54 clocks. 24 and 36 are recommended
    driver.blank_time(24);  // clocks

    // Set driver microsteps, [1-255]
    driver.microsteps(16);

    // 20bit max. Lower velocity threshold for switching on smart energy coolStep and stall guard
    // Needs to be enabled before activating DIAG1 on motor stall
    driver.TCOOLTHRS(0xFFFFF);

    // This velocity setting allows velocity dependent switching into a different chopper mode and
    // full stepping to maximize torque. (unsigned)
    // The stall detection feature becomes switched off for 2-3 electrical periods whenever passing
    // THIGH threshold to compensate for the effect of switching modes.
    driver.THIGH(0);

    // Set the max hardware current
    driver.rms_current(600);  // mA

    // Enable stealth chop, extremely quiet stepping
    driver.en_pwm_mode(HIGH);

    // Enable automatic current control
    // Attention: When using a user defined sine wave table, the amplitude of this sine wave table should
    // not be less than 244. Best results are obtained with 247 to 252 as peak values.
    driver.pwm_autoscale(HIGH);

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
    driver.sgt(15);

    // Enable DIAG1 active on motor stall (set TCOOLTHRS before using this feature)
    driver.diag1_stall(true);

    attachInterrupt(digitalPinToInterrupt(diag1), Flower::motorStalled, RISING);
}

void Flower::setup_stepper() {
    stepper.setEnablePin(enable);
    stepper.setPinsInverted(false, false, true);
    stepper.enableOutputs();
}

// ---- Accessor Functions ----

long Flower::remainingDistance() {
    return stepper.distanceToGo();
}

// ---- Modifier Functions ----

void Flower::setMaxSpeed(float speed) {
    stepper.setMaxSpeed(speed);
}

void Flower::setAcceleration(float acceleration) {
    stepper.setAcceleration(acceleration);
}

// ---- Actions ----

bool Flower::run()
{
    return stepper.run();
}

void Flower::move(int mm) {
    stepper.move(mm * steps_per_mm);
}

void Flower::reverse() {
    driver.shaft(!driver.shaft());
}

// ---- Stall Detection ----

void Flower::motorStalled() {
    stalled = true;
}
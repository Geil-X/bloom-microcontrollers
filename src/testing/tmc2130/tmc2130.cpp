#include <Arduino.h>
#include <Wire.h>
#include <TMCStepper.h>
#include <AccelStepper.h>
#include "Interrupt.h"

// ---- Initialization ----

// Stepper Pins
#define EN_PIN      7
#define DIR_PIN     8
#define STEP_PIN    9
#define CS_PIN      10

// SPI Communication
#define SW_MOSI    11  // SDI
#define SW_MISO    12  // SDO
#define SW_SCK     13  // SPI Reference Clock

// TMC2130 Parameters
#define R_SENSE 0.11f  // Set for the silent step stick series

// I2cController Libraries
TMC2130Stepper driver(CS_PIN, R_SENSE, SW_MOSI, SW_MISO, SW_SCK);
AccelStepper stepper(stepper.DRIVER, STEP_PIN, DIR_PIN);

ISR(TIMER1_COMPA_vect) {
    stepper.runSpeed();
}

// Stall detection
bool stalled() {
    return driver.sg_result() < 950;
}

// ---- Main Functions ----

void setup() {
    // Driver Initialization
    pinMode(CS_PIN, OUTPUT);
    pinMode(EN_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);
    pinMode(STEP_PIN, OUTPUT);

    digitalWrite(CS_PIN, HIGH);
    digitalWrite(EN_PIN, HIGH);
    digitalWrite(DIR_PIN, LOW); //LOW or HIGH
    digitalWrite(STEP_PIN, LOW);
    pinMode(MISO, INPUT_PULLUP);

    driver.begin();
    driver.toff(4);  // us
    driver.tbl(1);
    driver.rms_current(300);
    driver.microsteps(16);
    driver.en_pwm_mode(true);
    driver.pwm_autoscale(true);
    driver.pwm_ampl(255);
    driver.hysteresis_start(4);
    driver.hysteresis_end(0);
    driver.chm(true); // Disable spread cycle
    driver.semin(5);
    driver.semax(2);
    driver.sedn(0b01);

    driver.TCOOLTHRS(0xFFFFF);
    driver.THIGH(0);

    // Stall detection
    driver.sgt(30);

    // Stepper Initialization
    stepper.setMaxSpeed(15000);
    stepper.setAcceleration(5000);
    stepper.setSpeed(5000);

    stepper.setEnablePin(EN_PIN);
    stepper.setPinsInverted(false, false, true);
    stepper.enableOutputs();

    setUpInterrupts(18);
}

void loop() {
//    if (stalled()) {
//        driver.shaft(!driver.shaft());
//    }
}

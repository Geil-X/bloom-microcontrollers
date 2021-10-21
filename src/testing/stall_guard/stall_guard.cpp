#include <Arduino.h>
#include <TMCStepper.h>
#include <AccelStepper.h>

// ---- Initialization ----

// Stepper Pins
#define DIAG1_PIN   3
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

// I2CController Libraries
TMC2130Stepper driver(CS_PIN, R_SENSE, SW_MOSI, SW_MISO, SW_SCK);
AccelStepper stepper(stepper.DRIVER, STEP_PIN, DIR_PIN);

// Stall detection
volatile bool stalled = false;

void onStall() {
    stalled = true;
}

// ---- Main Functions ----

void setup() {
    // Driver Initialization
    pinMode(DIAG1_PIN, INPUT);
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
    driver.hysteresis_start(0);
    driver.semin(5);
    driver.semax(2);
    driver.sedn(0b01);

    driver.TCOOLTHRS(0xFFFFF);
    driver.THIGH(0);

    // Stall detection
    driver.sgt(60);
    driver.diag1_stall(true);
    driver.diag1_pushpull(true);  // Active high

    attachInterrupt(digitalPinToInterrupt(DIAG1_PIN), onStall, RISING);

    // Stepper Initialization
    stepper.setMaxSpeed(10000);
    stepper.setAcceleration(5000);
    stepper.setSpeed(5000);

    stepper.setEnablePin(EN_PIN);
    stepper.setPinsInverted(false, false, true);
    stepper.enableOutputs();
}

void loop() {
    if (stalled) {
        driver.shaft(!driver.shaft());
        stalled = false;
    }

    stepper.runSpeed();
}

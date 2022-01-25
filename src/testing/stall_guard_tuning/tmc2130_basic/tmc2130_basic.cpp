#include <Arduino.h>
#include <TMCStepper.h>
#include <AccelStepper.h>
#include <MathExtra.h>
#include <Wire.h>
#include <SerialTransfer.h>
#include <Potentiometer.h>
#include <Button.h>
#include "Interrupt.h"

SerialTransfer serialTransfer;

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

// Stall Detection
#define STALL_REFRESH_RATE 5
#define STALL_BUFFER 150

unsigned long last_stall = 0;
unsigned long stall_buffer = 0;


struct ControlParams {
    int speed = 0;
    int stall_guard_threshold = 0;
    int minimum_stall_threshold = 0;
    int stall_value = 0;
} control;

Potentiometer speedPot = Potentiometer(A3, 0, 10000);
Potentiometer sgtPot = Potentiometer(A4, 5, 63);
Potentiometer minimumStallThresholdPot = Potentiometer(A5, 1, 1022);
Button enableButton = Button(2);

bool enabled = false;

ISR(TIMER1_COMPA_vect) {
    if (enabled) { stepper.runSpeed(); }
}

// ---- Main Functions ----

void setup() {
    Serial.begin(115200);
    serialTransfer.begin(Serial);

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
    driver.chm(true);
    driver.pwm_ampl(255);
    driver.hysteresis_start(4);
    driver.hysteresis_start(0);
    driver.semin(5);
    driver.semax(2);
    driver.sedn(0b01);

    driver.TCOOLTHRS(0xFFFFF);
    driver.THIGH(0);

    // Stall detection
    driver.sgt(30);

    // Stepper Initialization
    stepper.setMaxSpeed(1000000);
    stepper.setAcceleration(500000);
    stepper.setSpeed(0);

    stepper.setEnablePin(EN_PIN);
    stepper.setPinsInverted(false, false, true);
    stepper.enableOutputs();

    setUpInterrupts(18);
}

void setControlParameters() {
    control.minimum_stall_threshold = minimumStallThresholdPot.value();
    control.speed = speedPot.value();
    control.stall_guard_threshold = sgtPot.value();

    stepper.setSpeed((float) control.speed);
    driver.sgt((int8_t) control.stall_guard_threshold);
}

__attribute__((unused)) void loop() {
    unsigned long ms = millis();

    if (ms - last_stall > STALL_REFRESH_RATE) {
        setControlParameters();
        control.stall_value = (int) driver.sg_result();

        if (ms - stall_buffer < STALL_BUFFER) {
//            control.stall_guard_threshold = 63;
//            driver.sgt(63);

        } else if (control.stall_value < control.minimum_stall_threshold) {
            driver.shaft(!driver.shaft());
            stall_buffer = ms;
        }

        last_stall = ms;
        serialTransfer.sendDatum(control);
    }

    if (enableButton.isPressed()) enabled = !enabled;
}

#include <Arduino.h>
#include <TMCStepper.h>
#include <AccelStepper.h>

// ---- Initialization ----

// Uart Communication
#define RX2 16
#define TX2 0

// Stepper Pins
#define DIAG1_PIN   3
#define EN_PIN      7
#define DIR_PIN     8
#define STEP_PIN    9

// TMC2130 Parameters
#define DRIVER_ADDRESS  0b00     // TMC2209 Driver address according to MS1 and MS2
#define SERIAL_PORT     Serial2  // TMC2208/TMC2224 HardwareSerial port
#define R_SENSE         0.11f    // Set for the silent step stick series
#define STALL_VALUE     50       // [0..255]

// I2CController Libraries
TMC2209Stepper driver(RX2, TX2, R_SENSE,DRIVER_ADDRESS);
AccelStepper stepper(stepper.DRIVER, STEP_PIN, DIR_PIN);

// Stall detection
volatile bool stalled = false;

void onStall() {
    stalled = true;
}

// ---- Main Functions ----

void setup() {
    driver.beginSerial(115200);

    // Driver Initialization
    pinMode(DIAG1_PIN, INPUT);
    pinMode(EN_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);
    pinMode(STEP_PIN, OUTPUT);

    digitalWrite(EN_PIN, HIGH);
    digitalWrite(DIR_PIN, LOW); //LOW or HIGH
    digitalWrite(STEP_PIN, LOW);

    driver.begin();
    driver.toff(4);  // us
    driver.tbl(1);
    driver.rms_current(300);
    driver.microsteps(16);
//    driver.en_pwm_mode(true);
    driver.pwm_autoscale(true);
//    driver.pwm_ampl(255);
    driver.hysteresis_start(4);
    driver.hysteresis_start(0);
    driver.semin(5);
    driver.semax(2);
    driver.sedn(0b01);
    driver.en_spreadCycle(false);

    driver.TCOOLTHRS(0xFFFFF);
//    driver.THIGH(0);

    // Stall detection
    driver.SGTHRS(STALL_VALUE);
//    driver.diag1_stall(true);
//    driver.diag1_pushpull(true);  // Active high

    attachInterrupt(digitalPinToInterrupt(DIAG1_PIN), onStall, RISING);

    // Stepper Initialization
    stepper.setMaxSpeed(10000);
    stepper.setAcceleration(5000);
    stepper.setSpeed(5000);

    stepper.setEnablePin(EN_PIN);
    stepper.setPinsInverted(false, false, true);
    stepper.enableOutputs();
    digitalWrite(EN_PIN, LOW);
}

__attribute__((unused)) void loop() {
    if (stalled) {
        driver.shaft(!driver.shaft());
        stalled = false;
    }

    stepper.runSpeed();
}

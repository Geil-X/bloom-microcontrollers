#include <Arduino.h>
#include <Wire.h>
#include <Logging.h>
#include <Flower.h>
#include <SerialTransfer.h>
#include <Potentiometer.h>
#include <Button.h>

// Pin Definitions for different microcontrollers

// Stepper Pins
#define DIAG1_PIN 3
#define EN_PIN    7
#define DIR       8
#define STEP      9
#define CS        10

// Stepper Driver SPI Communication
#define MOSI    11  // SDI
#define MISO    12  // SDO
#define SCK     13  // SPI Reference Clock

// Sensor Pins
#define MOTOR_VOLTAGE A0

// Communication Rate
#define COMMUNICATION_DELAY 1 // ms
unsigned long last_communication = 0;

Flower flower = Flower(EN_PIN, DIR, STEP, CS, MOSI, MISO, SCK, DIAG1_PIN, MOTOR_VOLTAGE);
SerialTransfer serialTransfer;

Potentiometer speedPot = Potentiometer(A3, 0, 10000);
Potentiometer sgtPot = Potentiometer(A4, 5, 63);
Potentiometer minimumStallThresholdPot = Potentiometer(A5, 1, 1022);
Button enableButton = Button(2);

bool enabled = true;

struct ControlParams {
    int speed = 0;
    int stall_guard_threshold = 0;
    int minimum_stall_threshold = 0;
    int stall_value = 0;
} control;

void setupFlower() {
    flower.setup();
//    flower.home();
    flower.setDirection(DIRECTION_OPEN);
    flower.setMaxSpeed(10000);
    flower.setAcceleration(5000);
    flower.setSpeed(0);
}

void setup() {
    Log::connect(Log::NONE, 115200);
    Serial.begin(115200);
    serialTransfer.begin(Serial);

    while (!flower.hasPower()) {}
    setupFlower();
}

void setControlParameters() {
    control.minimum_stall_threshold = minimumStallThresholdPot.value();
    control.speed = speedPot.value();
    control.stall_guard_threshold = sgtPot.value();
    control.stall_value = (int) flower.getStallGuardResult();

    flower.setSpeed((float) control.speed);
    flower.setStallGuardThreshold(control.stall_guard_threshold);
    flower.setStallDetectionThreshold(control.minimum_stall_threshold);
}

__attribute__((unused)) void loop() {
    if (millis() - last_communication > COMMUNICATION_DELAY) {
        setControlParameters();
        serialTransfer.sendDatum(control);
        last_communication = millis();

        if (flower.regainedPower()) { setupFlower(); }
        if (enableButton.isPressed()) { enabled = !enabled; }
    }
    if (flower.motorStalled()) { flower.reverse(); }
    if (enabled) { flower.runSpeed(); }
}

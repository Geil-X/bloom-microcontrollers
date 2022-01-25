#include <Arduino.h>
#include <Wire.h>
#include <Flower.h>
#include <SerialTransfer.h>
#include <Potentiometer.h>
#include <Button.h>
#include <Interrupt.h>
#include <Logging.h>

#define TRANSMISSION_RATE 5 // ms

// Pin Definitions for different microcontrollers

// Stepper Pins
#define EN_PIN    7
#define DIR       8
#define STEP      9
#define CS        10

// Stepper Driver SPI Communication
#define MOSI    11  // SDI
#define MISO    12  // SDO
#define SCK     13  // SPI Reference Clock

Flower flower = Flower(EN_PIN, DIR, STEP, CS, MOSI, MISO, SCK);
SerialTransfer serialTransfer;

Potentiometer speedPot = Potentiometer(A3, 0, 15000);
Potentiometer sgtPot = Potentiometer(A4, 0, 63);
Potentiometer minimumStallThresholdPot = Potentiometer(A5, 1, 1022);
Button enableButton = Button(2);
Button homeButton = Button(4);

bool enabled = true;
unsigned long last_transfer = 0;

ISR(TIMER1_COMPA_vect) {
    if (enabled) { flower.runSpeed(); }
}

struct ControlParams {
    int speed = 0;
    int stall_guard_threshold = 0;
    int minimum_stall_threshold = 0;
    int stall_value = 0;
} control;

void setupFlower() {
    flower.setup();
    flower.setMaxSpeed(10000);
    flower.setAcceleration(5000);
    flower.setSpeed(3000);
}

void setup() {
    Log::connect(Log::PRINT);
    Serial.begin(115200);
    serialTransfer.begin(Serial);
    setupFlower();

    setUpInterrupts(18);
}

void setControlParameters() {
    flower.updateStallParameters();
    control.minimum_stall_threshold = (int) flower.getStallDetectionThreshold();
    control.speed = speedPot.value();
    control.stall_guard_threshold = flower.getStallGuardThreshold();
    control.stall_value = (int) flower.getStallGuardResult();

    flower.setSpeed((float) control.speed);
//    flower.setStallGuardThreshold(control.stall_guard_threshold);
//    flower.setStallDetectionThreshold(control.minimum_stall_threshold);
}

__attribute__((unused)) void loop() {
    unsigned long ms = millis();
    if (ms - last_transfer > TRANSMISSION_RATE) {
        setControlParameters();
        serialTransfer.sendDatum(control);
        last_transfer = ms;
    }

    if (enableButton.isPressed()) { enabled = !enabled; }
    if (homeButton.isPressed()) {
        enabled = false;
        flower.home();
    }

    if (flower.motorStalled()) { flower.reverse(); }

}
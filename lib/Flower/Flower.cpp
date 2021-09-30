#include "Flower.h"

Flower::Flower(uint8_t en, uint8_t dir, uint8_t step, uint8_t cs, uint8_t mosi, uint8_t miso, uint8_t sck, float rSense)
        : driver(cs, rSense, mosi, miso, sck)
        , stepper(stepper.DRIVER, step, dir)
        {

    // Pin initialization
    this->en = en;
    this->dir = dir;
    this->step = step;
    this->cs = cs;
    this->rSense = rSense;
    this->mosi = mosi;
    this->miso = miso;
    this->sck = sck;
}

void Flower::setup() {
    pinMode(cs, OUTPUT);
    digitalWrite(cs, HIGH);

    driver.begin();             // Initiate pins and registries
    driver.rms_current(600);  // Set the hardware current
    driver.en_pwm_mode(1);      // Enable extremely quiet stepping
    driver.pwm_autoscale(1);
    driver.microsteps(16);

    stepper.setMaxSpeed(50*steps_per_mm); // 100mm/s @ 80 steps/mm
    stepper.setAcceleration(1000*steps_per_mm); // 2000mm/s^2
    stepper.setEnablePin(en);
    stepper.setPinsInverted(false, false, true);
    stepper.enableOutputs();
}

void Flower::run() {
    if (stepper.distanceToGo() == 0) {
        stepper.disableOutputs();
        delay(100);
        stepper.move(100*steps_per_mm); // Move 100mm
        stepper.enableOutputs();
    }
    stepper.run();
}

void Flower::setSpeed(float speed) {
    stepper.setSpeed(speed);
}

void Flower::setMaxSpeed(float speed) {
    stepper.setMaxSpeed(speed);
}

void Flower::setAcceleration(float acceleration) {
    stepper.setAcceleration(acceleration);
}
/*
  Flower.cpp - Library for single flower node.
  Created by Ramon Qu.
*/

#include "Arduino.h"
#include "Flower.h"

const int INIT_TOTAL_STEP = 1600;
const long INIT_STEP_PER_SEC = 50000; // Steps / second
const long INIT_ACCELERATION = 50000;        // Steps / second^2

// The distance bellow which a stall is detected. This is relative to the petal rotation from the rotary sensor.
const int STALL_THRESHOLD = 5;
// The distance that the motor is moved away from where the stall occurred
const int BOUNDARY_OFFSET = 100;
// The distance movement distance between stall detection checks
const int STALL_DETECTION_MOVE_BLOCK = 100;


Flower::Flower() : Flower(-1, -1, -1) {}

Flower::Flower(int dir_pin, int step_pin, int sensor_pin) {
    _dir_pin = dir_pin;
    _step_pin = step_pin;
    stepper.setup(_step_pin, _dir_pin);
    _sensor_pin = sensor_pin;
    _dir = Open;
    _rate = 10;
    _lastSum = 0;
    _count = 0;
    _sum = 0;
    _isrunning = false;
    this->current_step = 0;
    total_step = INIT_TOTAL_STEP;
    _stall_threshold = STALL_THRESHOLD;
    _boundary_offset = BOUNDARY_OFFSET;
    _stall_detection_move_block = STALL_DETECTION_MOVE_BLOCK;
}

void Flower::stat() {
    Serial.print("Sensor Pin: ");
    Serial.print(_sensor_pin);
    Serial.print(" Dir Pin: ");
    Serial.print(_dir_pin);
    Serial.print(" step pin: ");
    Serial.println(_step_pin);
}

void Flower::setupThreshold(int stall_threshold, int boundary_offset, int stall_detection_move_block) {
    _stall_threshold = stall_threshold;
    _boundary_offset = boundary_offset;
    _stall_detection_move_block = stall_detection_move_block;
}

/**
 * Initializes the speed and acceleration of the motor.
 */
void Flower::setup() {
    setRate(INIT_STEP_PER_SEC, INIT_ACCELERATION);
}

/**
 * Motor Speed range:  1 - 300000
 */
void Flower::setSpeed(int speed) {
    stepper.setMaxSpeed(speed);
}

/**
 * Acceleration range: 0 - 500000
 */
void Flower::setAcceleration(int acceleration) {
    stepper.setAcceleration(acceleration);
}

/**
 * Motor Speed range:  1 - 300000
 * Acceleration range: 0 - 500000
 */
void Flower::setRate(int speed, int acceleration) {
    setSpeed(speed);
    setAcceleration(acceleration);
}

void Flower::step() { step(1); }

void Flower::step(int steps) {
    _isrunning = true;
    stepper.setTargetRel(steps);
    controller.move(stepper);
    this->current_step -= steps;
    _isrunning = false;
}

void Flower::reverse() {
    _dir = _dir == Open ? Close : Open;
}

/**
 * Move until a stall is detected. For consistency, several consecutive stalls
 * are checked to ensure that the stalls occurred at the edge of the movement.
 *
 * @param direction Either 1 or -1 indicating the direction of movement.
 * @return The number of steps that it took to move to the stall. This doesn't
 *  include the number of steps that were taken while stalling.
 */
int Flower::moveUntilStall(Direction direction) {
    const int REQUIRED_CONSECUTIVE_STALLS = 4;
    const int BOUNDARY_STEPS = 3;
    int encoder_reading = 0;
    int previous_encoder_reading = 0;
    int steps = 0;
    int stall_count = 0;

    while (true) {
        int sensor_delta = abs(encoder_reading - previous_encoder_reading);
        bool stall_detected = sensor_delta != 0 && sensor_delta < _stall_threshold;

        if (stall_detected) {
            stall_count++;
            // Exit condition
            if (stall_count >= REQUIRED_CONSECUTIVE_STALLS) {
                // Reverse the motor a single step
                stepper.setTargetRel(-direction * BOUNDARY_STEPS * _stall_detection_move_block);
                controller.move(stepper);

                return steps - BOUNDARY_STEPS;
            }
        } else {
            // Increment the number of steps only if the motor did not stall
            steps += _stall_detection_move_block;
        }

        // Move a single step
        stepper.setTargetRel(direction * _stall_detection_move_block);
        controller.move(stepper);

        // Record the encoder movement of a single step
        previous_encoder_reading = encoder_reading;
        encoder_reading = analogRead(_sensor_pin);
    }
}

int Flower::moveUntilStall() { return moveUntilStall(Open); }

void Flower::home() {
    total_step = moveUntilStall(Open);
    total_step = moveUntilStall(Close);
    current_step = total_step;
}

void Flower::open() {
    step(current_step);
}

void Flower::close() {
    step(current_step - total_step);
}

int Flower::open(int percentage) {
    percentage = constrain(percentage, 0, 100);
    long target = (long) (total_step * (percentage / 100.));
    step(current_step - target);
    return percentage;
}

void Flower::setDir(Direction dir) {
    _dir = dir;
}

bool Flower::operator==(Flower &other) const {
    return this->_dir_pin == other._dir_pin &&
           this->_step_pin == other._step_pin &&
           this->_sensor_pin == other._sensor_pin;
}

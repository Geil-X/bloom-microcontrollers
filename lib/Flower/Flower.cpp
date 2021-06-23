/*
  Flower.cpp - Library for single flower node.
  Created by Ramon Qu.
*/

#include "Arduino.h"
#include "Flower.h"
#include "TeensyStep.h"
#include <TeensyThreads.h>

const int INIT_TOTAL_STEP = 1600;
const long INIT_STEP_PER_SEC = 50000; // Steps / second
const long INIT_ACCELERATION = 50000;        // Steps / second^2

const int STALL_THRESHOLD = 5;
const int BOUNDARY_OFFSET = 100;
const int STALL_DETECTION_MOVE_BLOCK = 100;


Flower::Flower() {}

Flower::Flower(int DIR_PIN, int STEP_PIN, int sensorpin) {
    _dir_pin = DIR_PIN;
    _step_pin = STEP_PIN;
    stepper.setup(_step_pin, _dir_pin);
    _sensor_pin = sensorpin;
    _dir = 1;
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

void Flower::step() {
    if (_dir == 0) {
        stepper.setTargetRel(-1);
        this->current_step += 1;
    } else {
        stepper.setTargetRel(1);
        this->current_step -= 1;
    }
}

void Flower::step(int steps) {
    if (_dir == 0) {
        stepper.setTargetRel(-1 * steps);
        this->current_step += steps;
    } else {
        stepper.setTargetRel(steps);
        this->current_step -= steps;
    }
}

void Flower::reverse() {
    _dir = 1 - _dir;
}

void Flower::moveUntilStall() {
    double sum = 0;
    double lastsum = 0;
    int dir = -1;
    int iscount = 0;
    int step = 0;
    while (true) {
        if ((sum - lastsum != 0) && abs(sum - lastsum) < _stall_threshold) {
            Serial.println("finished");
            return;
        }
        lastsum = analogRead(_sensor_pin);
        stepper.setTargetRel(dir * _stall_detection_move_block);
        controller.move(stepper);
        if (iscount > 0) {
            step += _stall_detection_move_block;
        }
        sum = analogRead(_sensor_pin);
    }
}

void Flower::home() {
    double sum = 0;
    double lastsum = 0;
    int dir = 1;
    int iscount = 0;
    int step = 0;
    while (true) {
        if ((sum - lastsum != 0) && abs(sum - lastsum) < _stall_threshold) {
            iscount += 1;
            if (iscount > 2) {
                total_step = step / 2;
                break;
            }
            if (dir == 1) {
                dir = -1;
            } else {
                dir = 1;
            }
            stepper.setTargetRel(dir * _boundary_offset);
            controller.move(stepper);
            delay(500);

        }
        lastsum = analogRead(_sensor_pin);
        stepper.setTargetRel(dir * _stall_detection_move_block);
        controller.move(stepper);
        if (iscount > 0) {
            step += _stall_detection_move_block;
        }

        sum = analogRead(_sensor_pin);
    }
}

void Flower::open() {
    _isrunning = true;
    stepper.setTargetRel(this->current_step);
    controller.move(stepper);
    this->current_step = 0;
    _isrunning = false;
}

void Flower::close() {
    _isrunning = true;
    stepper.setTargetRel(this->current_step - total_step);
    controller.move(stepper);
    this->current_step = total_step;
    _isrunning = false;
}

int Flower::open(int percentage) {
    _isrunning = true;
    long target = (long) total_step * percentage / 100;
    if (this->current_step < target) {
        setDir(false);
        while (this->current_step < target) {
            step();
        }
    } else {
        setDir(true);
        while (this->current_step > target) {
            step();
        }
    }
    _isrunning = false;
    return percentage;
}

void Flower::setDir(bool open) {
    if (open) {
        _dir = 1;
    } else {
        _dir = 0;
    }
}

bool Flower::operator==(Flower &other) const {
    return this->_dir_pin == other._dir_pin &&
           this->_step_pin == other._step_pin &&
           this->_sensor_pin == other._sensor_pin;
}

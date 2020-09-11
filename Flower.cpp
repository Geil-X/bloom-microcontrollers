/*
  Flower.cpp - Library for single flower node.
  Created by Ramon Qu.
*/

#include "Arduino.h"
#include "Flower.h"
#include "TeensyStep.h"
#include <TeensyThreads.h>

const int INIT_TOTAL_STEP = 1600;
const long INIT_STEP_PER_SEC = 50000;
const long INIT_ACCEL = 200000;

const int STALL_THREHOLD = 8;
const int BOUNDRY_OFFSET = 40;
const int STALL_DETECTION_MOVE_BLOCK = 40;


Flower::Flower(){}

Flower::Flower(int DIR_PIN, int STEP_PIN, const uint8_t& sensorpin){
  _dirpin = DIR_PIN;
  _steppin = STEP_PIN;
  stepper.setup(_steppin, _dirpin);
  _sensorpin = sensorpin;
  _dir = 1;
  _rate = 30;
  _lastSum = 0;
  _count = 0;
  _sum = 0;
  _isrunning = false;
  this->current_step = 0;
  total_step = INIT_TOTAL_STEP;
  _stall_threhold = STALL_THREHOLD;
  _boundry_offset = BOUNDRY_OFFSET;
  _stall_detection_move_block = STALL_DETECTION_MOVE_BLOCK;
}

void Flower::setupThrehold(int stall_threhold, int boundry_offset, int stall_detection_move_block){
  _stall_threhold = stall_threhold;
  _boundry_offset = boundry_offset;
  _stall_detection_move_block = stall_detection_move_block;
}

void Flower::setup() {
  stepper.setMaxSpeed(INIT_STEP_PER_SEC)       // stp/s
        .setAcceleration(INIT_STEP_PER_SEC);
}

void Flower::setRate(int rate){
  //Motor Speed range 1 - 300000, 
  //accel range  0 - 500000
  stepper.setMaxSpeed(rate).setAcceleration(int(rate / 3.0 * 5));
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

void Flower::moveUntilStall(){
  double sum = 0;
  double lastsum = 0;
  int dir = -1;
  int iscount = 0;
  int step = 0;
  while(true){
    if ((sum -lastsum != 0) && abs(sum  - lastsum ) < _stall_threhold) {
      Serial.println("finished");
      return;
    }
    lastsum =  analogRead(_sensorpin);
    stepper.setTargetRel(dir*_stall_detection_move_block);
    controller.move(stepper);
    if(iscount > 0){
      step += _stall_detection_move_block;
    }
    sum = analogRead(_sensorpin);
  }
}

void Flower::home() {
  double sum = 0;
  double lastsum = 0;
  int dir = 1;
  int iscount = 0;
  int step = 0;
  while (true) {
    if ((sum -lastsum != 0) && abs(sum  - lastsum ) < _stall_threhold) {
      iscount += 1;
      if(iscount > 2){
        total_step = step / 2;
        break;
      }
      if (dir == 1) {
        dir = -1;
      } else {
        dir = 1;
      }
      stepper.setTargetRel(dir * _boundry_offset);
      controller.move(stepper);
      delay(500);
     
    }
    lastsum =  analogRead(_sensorpin);
    stepper.setTargetRel(dir*_stall_detection_move_block);
    controller.move(stepper);
    if(iscount > 0){
      step += _stall_detection_move_block;
    }
    
    sum = analogRead(_sensorpin);
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

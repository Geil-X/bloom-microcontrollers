/*
  Flower.cpp - Library for single flower node.
  Created by Ramon Qu.
*/

#include "Arduino.h"
#include "Flower.h"
#include "TeensyStep.h"

Flower::Flower(int DIR_PIN, int STEP_PIN, const uint8_t& sensorpin){
  _dirpin = DIR_PIN;
  _steppin = STEP_PIN;
  stepper.setup(_steppin, _dirpin);
  //Stepper stepper(_steppin, _dirpin);
  _sensorpin = sensorpin;
  _dir = 1;
  _rate = 30;
  _lastSum = 0;
  _count = 0;
  _sum = 0;
  _isrunning = false;
  current_step = 0;
  total_step = 1600;
}

Flower::Flower(int DIR_PIN,int STEP_PIN,int MS1,int MS2,int SLP,const uint8_t& sensorpin){
  _dirpin = DIR_PIN;
  _steppin = STEP_PIN;
  //Stepper stepper(_steppin, _dirpin);
  _sensorpin = sensorpin;
  _dir = 1;
  _rate = 30;
  _lastSum = 0;
  _count = 0;
  _sum = 0;
  _isrunning = false;
  current_step = 0;
  total_step = 1600;
}
void Flower::setup() {
  
  stepper.setMaxSpeed(50000)             // stp/s
        .setAcceleration(200000);
}

void Flower::step() {
    if (_dir == 0) {
      stepper.setTargetRel(-1);
      current_step += 1;
    } else {
      stepper.setTargetRel(1);
      current_step -= 1;
    }
}

void Flower::step(int steps) {
    if (_dir == 0) {
      stepper.setTargetRel(-1 * steps);
      current_step += steps;
    } else {
      stepper.setTargetRel(steps);
      current_step -= steps;
    }
}

void Flower::slowlyOpen() {
  _isrunning = true;
  setDir(true);
  while (current_step > 0) {
    setRate((int) current_step / 1600.0 * 90.0 + 10);
    step();
  }
  setRate(10);
  _isrunning = false;
}

void Flower::slowlyClose() {
  _isrunning = true;
  setDir(false);
  while (current_step < total_step) {
    setRate((int) current_step / 1600.0 * 90.0 + 10);
    step();
  }
  setRate(10);
  _isrunning = false;
}


void Flower::reverse() {
  _dir = 1 - _dir;
}

void Flower::setRate(int rate) {
  //stepper.setSpeedRPM(rate);
}

void Flower::home() {

  double sum = 0;
  double lastsum = 0;
  int dir = 1;
  int iscount = 0;
  int step = 0;


  while (true) {

    if ((sum -lastsum != 0) && abs(sum  - lastsum ) < 40) {
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
      stepper.setTargetRel(dir * 100);
      controller.move(stepper);
      delay(500 );
     
    }
    
    lastsum =  analogRead(A9);
    stepper.setTargetRel(dir*100);
    controller.move(stepper);
    step +=50;
    sum = analogRead(A9);
  }
}

void Flower::open() {
  _isrunning = true;
  stepper.setTargetRel(current_step);
  controller.move(stepper);
  current_step = 0;
  _isrunning = false;
}

void Flower::close() {
  _isrunning = true;
  stepper.setTargetRel(current_step - total_step);
  controller.move(stepper);
  current_step = total_step;
  _isrunning = false;
}

int Flower::open(int percentage) {
  _isrunning = true;
  long target = (long) total_step * percentage / 100;
  if (current_step < target) {
    setDir(false);
    while (current_step < target) {
      step();
    }
  } else {
    setDir(true);
    while (current_step > target) {
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
/*
  Flower.cpp - Library for single flower node.
  Created by Ramon Qu.
*/

#include "Arduino.h"
#include "Flower.h"
#include <AH_EasyDriver.h>
   

AH_EasyDriver stepper;
long current_step = 0;
long total_step = 1600;

IntervalTimer myTimer;
int _sensorpin;
double _sensorvalue;
bool _isrunning;
float _rate;
float _stepToSensorVal[5000] = {};

Flower::Flower(int DIR_PIN, int STEP_PIN, int MS1, int MS2, int SLP,const uint8_t& sensorpin) {
  _driver = 1;
  //_enpin = EN_PIN;
  _dirpin = DIR_PIN;
  _steppin = STEP_PIN;
  _ms1pin = MS1;
  //_cspin = CS_PIN;
  _ms2pin = MS2;
  _slppin = SLP;
  _sensorpin = sensorpin;
  _dir = 1;
  _rate = 10;
  _function = false;
  _sensorvalue = 0;
  _isrunning = false;
  stepper.customSet(200, _dirpin, _steppin, _ms1pin, _ms2pin, _slppin);
}


void Flower::setup() {
  
  stepper.sleepOFF();
  stepper.setMicrostepping(3);
  stepper.setSpeedRPM(_rate);
  Serial.begin(9600);
  //myTimer.begin(logSensorValue, 200000);
}

void Flower::logSensorValue(){
  Serial.println(_isrunning);
  if(_isrunning){
    if(analogRead(_sensorpin) < _stepToSensorVal[current_step] *0.8 ){  //(60 /5* 5.68*0.6)
      Serial.println("Stall Detected");
      continueUntilStall();
      moveToCurrrentStep();
    }
  }
  Serial.println(analogRead(_sensorpin));
}

void Flower::moveToCurrrentStep(){
  for (int i=0;i<current_step;i++){
    stepper.move(1);
  }
}
void Flower::step() {
  if (_function) {
    if (_dir == 0) {
      stepper.move(-1);
      current_step += 1;
    } else {
      stepper.move(1);
      current_step -= 1;
    }
  } else {
    if (_dir == 0) {
      stepper.move(-1);
      current_step += 1;
    } else {
      stepper.move(1);
      current_step -= 1;
    }
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

void Flower::enableFunction(bool enable) {
  if (enable) {
    _function = true;
  } else {
    _function = false;
  }
}


void Flower::reverse() {
  _dir = 1 - _dir;
  digitalWrite(_dirpin, _dir);
}

void Flower::setRate(int rate) {
  stepper.setSpeedRPM(rate);
}
void Flower::home() {

  double sum = 0;
  double lastsum = 0;
  int count = 0;
  int dir = 1;
  int iscount = 0;
  double step = 0;

  while (true) {
    if (count > 20) {
      if (abs(sum / count - lastsum / count) < 4) {
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
        stepper.move(dir * 100);
        step += 80;
        delay(20);
      }
      count = 0;
      lastsum = sum;
      sum = analogRead(_sensorpin);
    } else {
      int temp = analogRead(_sensorpin);
      sum += temp;
      count += 1;
    }
    stepper.move(dir);
    if(iscount > 0){
      
      step ++;
    }
  }
  recordStepSensorValue();
  for(int i=0;i<total_step;i++){
    Serial.println(_stepToSensorVal[i]);
  }
  
}

void Flower::recordStepSensorValue(){
  Serial.println("record");
  setDir(false);
  setRate(5);
  while (current_step < total_step) {
    step();
    float temp = (float)analogRead(_sensorpin);
    Serial.print(current_step);
    Serial.print(" ");
    Serial.println(temp);
    _stepToSensorVal[current_step] = temp;
  }
}

void Flower::continueUntilStall() {
 double sum = 0;
  double lastsum = 0;
  int count = 0;
  int dir = -1;

  while (true) {
    if (count > 20) {
      if (abs(sum / count - lastsum / count) < 3) {
        if (dir == 1) {
          dir = -1;
        } else {
          dir = 1;
        }
        stepper.move(dir * 20);
        break;
      }
      count = 0;
      lastsum = sum;
      sum = analogRead(_sensorpin);
    } else {
      int temp = analogRead(_sensorpin);
      sum += temp;
      count += 1;
    }
    stepper.move(dir);
  }

}

void Flower::open() {
  _isrunning = true;
  setDir(true);
  while (current_step > 0) {
    step();
  }
  _isrunning = false;
}

void Flower::close() {
  _isrunning = true;
  setDir(false);
  while (current_step < total_step) {
    step();
  }
  _isrunning = false;
}

void Flower::open(int percentage) {
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
}

void Flower::setDir(bool open) {
  if (open) {
    _dir = 1;
  } else {
    _dir = 0;
  }

}
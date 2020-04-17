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
float _stepToSensorVal[5000];
float _lastSum;
float _count;
float _sum;

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
  _rate = 30;
  _function = false;
  _lastSum = 0;
  _count = 0;
  _sum = 0;
  _sensorvalue = 0;
  _isrunning = false;
  stepper.customSet(200, _dirpin, _steppin, _ms1pin, _ms2pin, _slppin);
}


void Flower::setup() {
  
  stepper.sleepOFF();
  stepper.setMicrostepping(3);
  stepper.setSpeedRPM(_rate);
  Serial.begin(9600);
  myTimer.begin(logSensorValue, 200000);
}

void Flower::logSensorValue(){
  if(_isrunning){
   
    float tmp = analogRead(_sensorpin);
    if(_count <5){
      _count += 1;
      _sum += tmp;
    }else{
      if(_lastSum == 0){
        _lastSum = _sum;
      }else{
        if(abs(_lastSum - _sum )>20 && abs(tmp - _stepToSensorVal[current_step])> 100 ){
          Serial.println("Stall Detected");
          Serial.print(tmp);
          Serial.print(" ");
          Serial.print(current_step);
          Serial.print(" ");
          Serial.print(_stepToSensorVal[current_step]);
          Serial.print(" ");
          Serial.print(_lastSum/5);
          Serial.print(" ");
          Serial.println(_sum/5);
          //_isrunning = false;
          //stepper.setSpeedRPM(20);
          //continueUntilStall();
          //Serial.println("Stalled, stop");
          //delay(5000);
          //moveToCurrrentStep();
          //Serial.println("Recovered");
          //delay(5000);
          //stepper.setSpeedRPM(_rate);
          //Serial.println("Recovered");
          //_isrunning = true;
        }
      }
      _count = 0;
    }
    /*if(abs(tmp - _stepToSensorVal[current_step] )> 200 ){  //(60 /5* 5.68*0.6)
      Serial.println("Stall Detected");
      Serial.print(tmp);
    Serial.print(" ");
    Serial.print(current_step);
    Serial.print(" ");
    Serial.println(_stepToSensorVal[current_step]);
      //continueUntilStall();
      //moveToCurrrentStep();
    }*/
  }
  
}

void Flower::moveToCurrrentStep(){
  Serial.println(current_step);
  for (int i=0;i< current_step;i++){
    stepper.move(-1);
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
      
      if (abs(sum  - lastsum ) < 80) {
        Serial.println(sum  - lastsum);
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
    Serial.print(i);
    Serial.print(" ");
    Serial.println(_stepToSensorVal[i]);
  }
  
}

void Flower::recordStepSensorValue(){
  setDir(false);
  setRate(10);
  while (current_step < total_step) {
    float temp = (float)analogRead(_sensorpin);
    _stepToSensorVal[current_step] = temp;
    Serial.print("record ");
    Serial.print(current_step);
    Serial.print(" ");
    Serial.println(_stepToSensorVal[current_step]);
    step();
  }
  float temp = (float)analogRead(_sensorpin);
    _stepToSensorVal[current_step] = temp;
    Serial.print("record ");
    Serial.print(current_step);
    Serial.print(" ");
    Serial.println(_stepToSensorVal[current_step]);
}

void Flower::continueUntilStall() {
Serial.println("Recovering");
 double sum = 0;
  double lastsum = 0;
  int count = 0;
  int dir = -1;

  while (true) {
    if (count > 5) {
      if (abs(sum  - lastsum ) < 80) {
        
        stepper.move(-1*dir * 20);
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
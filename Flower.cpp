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


Flower::Flower(int DIR_PIN, int STEP_PIN, int MS1, int MS2, int SLP) {
  _driver = 1;
  //_enpin = EN_PIN;
  _dirpin = DIR_PIN;
  _steppin = STEP_PIN;
  _ms1pin = MS1;
  //_cspin = CS_PIN;
  _ms2pin = MS2;
  _slppin = SLP;
  _sensorpin = A9;

  _dir = 1;
  _rate = 10;
  _function = false;
  stepper.customSet(200, _dirpin, _steppin, _ms1pin, _ms2pin, _slppin);
}


void Flower::setup() {
  
  stepper.sleepOFF();
  stepper.setMicrostepping(3);
  stepper.setSpeedRPM(_rate);
  //stepper(200,2,3,4,5,6);    // init w/o "enable" and "reset" functions

}
int test(int x) {
  return x * x;
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
  setDir(true);
  while (current_step > 0) {
    setRate((int) current_step / 1600.0 * 90.0 + 10);
    step();
  }
  setRate(10);
}

void Flower::slowlyClose() {
  setDir(false);
  while (current_step < total_step) {
    setRate((int) current_step / 1600.0 * 90.0 + 10);
    step();
  }
  setRate(10);
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
      if (abs(sum / count - lastsum / count) < 3) {
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
}

void Flower::continueUntilStall() {

 double sum = 0;
  double lastsum = 0;
  int count = 0;
  int dir = _dir;

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
  //Going down
  setDir(true);
  while (current_step > 0) {
    step();
  }
}

void Flower::close() {
  //Going up
  setDir(false);
  while (current_step < total_step) {
    step();
  }


}

void Flower::open(int percentage) {
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

}

void Flower::setDir(bool open) {
  if (open) {
    _dir = 1;
  } else {
    _dir = 0;
  }

}
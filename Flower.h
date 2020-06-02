/*
  Flower.h - Library for controlling single flower code.
  Created by Ramon Qu.
*/
#ifndef Flower_h
#define Flower_h

#include "Arduino.h"
#include "TeensyStep.h"

class Flower
{
  public:
    Flower(int DIR_PIN,int STEP_PIN,int MS1,int MS2,int SLP,const uint8_t& sensorpin);
    Flower(int DIR_PIN,int STEP_PIN,const uint8_t& sensorpin);
    void setup();
    void step();
    void step(int steps);
    void reverse();
    void setRate(int rate);
    void home();
    void open();
    int open(int percentage);
    void close();
    void setDir(bool open);
    void slowlyOpen();
    void slowlyClose();
    void recordStepSensorValue();
    
    int _dirpin;
    int _steppin;
    Stepper stepper;
    StepControl controller;
    long total_step;
  private:
  int _enpin;
    int _cspin;
    int _mosipin;
    int _misopin;
    int _sckpin;
    int _ms1pin;
    int _ms2pin;
    int _slppin;
    int _dir;
    int _driver;
    int _sensorpin;
    uint32_t _lasttime;
    long current_step;
    
    bool _isrunning;
    float _rate;
    float _stepToSensorVal[2000];
    float _lastSum;
    float _count;
    float _sum;
};

#endif
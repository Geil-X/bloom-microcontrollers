/*
  Flower.h - Library for controlling single flower code.
  Created by Ramon Qu.
*/
#ifndef Flower_h
#define Flower_h

#include "Arduino.h"
#include "TeensyStep.h"

class Flower {
public:
    Flower();

    Flower(int DIR_PIN, int STEP_PIN, int sensorpin);

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

    void setupThrehold(int stall_threhold, int boundry_offset, int stall_detection_move_block);

    void recordStepSensorValue();

    void moveUntilStall();

    void stat();

    bool operator==(Flower &other) const;

    int _dirpin;
    int _steppin;
    Stepper stepper;
    StepControl controller;
    int total_step;
    int _sensorpin;
    int current_step;

private:
    int _stall_threhold;
    int _boundry_offset;
    int _stall_detection_move_block;
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

    uint32_t _lasttime;


    bool _isrunning;
    float _rate;
    float _stepToSensorVal[2000];
    float _lastSum;
    float _count;
    float _sum;
};

#endif
/*
  Flower.h - Library for controlling single flower code.
  Created by Ramon Qu.
*/
#ifndef Flower_h
#define Flower_h

#include "Arduino.h"
#include "TeensyStep.h"

enum Direction {
    Open = 1,
    Close = -1
};

class Flower {
public:
    Flower();

    Flower(int dir_pin, int step_pin, int sensorpin);

    void setup();

    void step();

    void step(int steps);

    void reverse();

    void setSpeed(int speed);

    void setAcceleration(int acceleration);

    void setRate(int speed, int acceleration);

    void home();

    void open();

    int open(int percentage);

    void close();

    void setDir(Direction dir);

    void setupThreshold(int stall_threshold, int boundary_offset, int stall_detection_move_block);

    void recordStepSensorValue();

    int moveUntilStall();
    int moveUntilStall(Direction direction);

    void stat();

    bool operator==(Flower &other) const;

    int _dir_pin;
    int _step_pin;
    Stepper stepper;
    StepControl controller;
    int total_step;
    int _sensor_pin;
    int current_step;

private:
    int _stall_threshold;
    int _boundary_offset;
    int _stall_detection_move_block;
    int _enpin;
    int _cspin;
    int _mosipin;
    int _misopin;
    int _sckpin;
    int _ms1pin;
    int _ms2pin;
    int _slppin;
    Direction _dir;
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
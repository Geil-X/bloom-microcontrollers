/*
  Flower.h - Library for controlling single flower code.
  Created by Ramon Qu.
*/
#ifndef FlowerGroup_h
#define FlowerGroup_h

#include "Arduino.h"
#include "Flower.h"
#include "TeensyStep.h"

class FlowerGroup
{
  public:
    FlowerGroup();
    void setup();
    void recordSensorVal();
    void addStepper(Flower& flower);
    void clear();
    void moveAbs(int target);
    void move(int target);
    

    float getSensorVal(Flower* f);
    static void checkStall();

    bool isRunning();

    

};

#endif
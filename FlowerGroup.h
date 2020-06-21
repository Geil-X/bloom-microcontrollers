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
    void addStepper(Flower& flower);
    void clear();
    void moveAbs(int target);
    void move(int target);
    Flower* flowers[10];
    int totalFlower;
    int workingFlowers;
    
    int working[10]= {0,0,0,0,0,0,0,0,0,0};
    StepControl controller;
};

#endif
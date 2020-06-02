/*
  Flower.h - Library for controlling single flower code.
  Created by Ramon Qu.
*/
#ifndef FlowerGroup_h
#define FlowerGroup_h

#include "Arduino.h"
#include "Flower.h"

class FlowerGroup
{
  public:
    FlowerGroup();
    void setup();
    void addStepper();
    void clear();
    void move();
 
};

#endif
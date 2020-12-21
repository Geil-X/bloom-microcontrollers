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
    void addStepper(Flower* flower);
    void clear();
    void home(int* motors);
    void moveAbs(int target);
    void move(int target);
    void move(int target, int* motors);
    void moveInd(int* target, int* motors);
    void setThrehold(int flowerid, int stall_threhold, int boundry_offset, int stall_detection_move_block);
    void reset();
    float getSensorVal(Flower* f);
    static void checkStall();

    bool isRunning();
    int numFlower();
    void stat();

    void forceStop();
    void softStop();

    

};

#endif
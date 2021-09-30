/*
  Flower.h - Library for controlling single flower code.
  Created by Ramon Qu.
*/
#ifndef FlowerGroup_h
#define FlowerGroup_h

#include <Arduino.h>
#include "Flower.h"

class FlowerGroup {
public:
    static void init();
    static void setup();
    static void recordSensorVal();
    static bool addStepper(Flower *flower);
    static void clear();
    static void home(int *motors);
    static void moveAbs(int target);
    static void move(int target);
    static void move(int target, int *motors);
    static void moveInd(int *target, int *motors);
    static void setSpeed(int speed);
    static void setAcceleration(int acceleration);
    static void setThreshold(int flower_id, int stall_threshold, int boundary_offset, int stall_detection_move_block);
    static void reset();
    static int getSensorVal(Flower *f);
    static void checkStall();
    static bool isRunning();
    static int numFlower();
    static void stat();
    static void forceStop();
    static void softStop();

private:
    static void moveAll(Stepper *workingMotors[], int count);
    static void moveAllAsync(Stepper *workingMotors[], int count);
    static bool containsFlower(Flower *f);
};

#endif
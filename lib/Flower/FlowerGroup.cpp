#include "FlowerGroup.h"

const int MAX_FLOWERS = 10;

IntervalTimer myTimer;
Flower *flowers[MAX_FLOWERS];
volatile int numFlowers = 0;
int workingFlowers = 0;

int working[MAX_FLOWERS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int finished[MAX_FLOWERS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
StepControl controller;
volatile int step2SensorVal[10][2000];

// stall detection variables
volatile int _count[MAX_FLOWERS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
volatile int _sum[MAX_FLOWERS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
volatile int _lastSum[MAX_FLOWERS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
volatile bool _isRunning = false;

// recover 0 means wait another motor to be 0 and catch up.
// recover 1 means go back to 0, and continue.
volatile int recoverMethod = 1;

void FlowerGroup::init() {
    myTimer.begin(checkStall, 1000);
}


bool FlowerGroup::isRunning() {
    return _isRunning;
}

int FlowerGroup::numFlower() {
    return numFlowers;
}

void FlowerGroup::setSpeed(int speed) {
    for (int i = 0; i < numFlowers; i++) {
        Flower *flower = flowers[i];
        flower->setSpeed(speed);
    }
}

void FlowerGroup::setAcceleration(int acceleration) {
    for (int i = 0; i < numFlowers; i++) {
        Flower *flower = flowers[i];
        flower->setAcceleration(acceleration);
    }
}

void FlowerGroup::setThreshold(
        int flower_id, int stall_threshold, int boundary_offset, int stall_detection_move_block) {
    if (flower_id >= 0 && flower_id < MAX_FLOWERS) {
        flowers[flower_id]->setupThreshold(stall_threshold, boundary_offset, stall_detection_move_block);
    }
}


void FlowerGroup::reset() {
    numFlowers = 0;
    workingFlowers = 0;
    _isRunning = false;
}

void FlowerGroup::recordSensorVal() {
    for (int i = 0; i < numFlowers; i++) {
        for (int j = 0; j < flowers[i]->total_step; j++) {
            flowers[i]->stepper.setTargetAbs(j);
            controller.move(*&flowers[i]->stepper);
            step2SensorVal[i][j] = analogRead(flowers[i]->_sensor_pin);
            delay(5);
        }
    }
}

int FlowerGroup::getSensorVal(Flower *f) {
    return analogRead(f->_sensor_pin);
}

void FlowerGroup::stat() {
    for (int i = 0; i < numFlowers; i++) {
        flowers[i]->stat();
    }
}

void FlowerGroup::checkStall() {
    if (_isRunning) {
        for (int i = 0; i < numFlowers; i++) {
            if (working[i] == 1) {
                int tmp = analogRead(flowers[i]->_sensor_pin);
                if (_count[i] < 12) {
                    _count[i] += 1;
                    _sum[i] += tmp;
                } else {
                    if (_lastSum[i] == 0) {
                        _lastSum[i] = _sum[i];
                    } else {
                        if (abs(_lastSum[i] - _sum[i]) < 8 &&
                            abs(tmp - step2SensorVal[i][flowers[i]->stepper.getPosition()]) > 300
                            && abs(flowers[i]->stepper.getPosition() - flowers[i]->total_step) > 200 &&
                            flowers[i]->stepper.getPosition() > 200) {
                            Serial.println("Stall Detected");
                            Serial.print(tmp);
                            Serial.print(" ");
                            Serial.print(abs(_lastSum[i] - _sum[i]));
                            Serial.print(" ");
                            Serial.print(flowers[i]->stepper.getPosition());
                            Serial.print(" ");
                            Serial.println(step2SensorVal[i][flowers[i]->stepper.getPosition()]);
                            working[i] = 0;
                        }
                        _lastSum[i] = _sum[i];
                        _count[i] = 0;
                        _sum[i] = 0;
                    }
                }
            }

        }
    }
}

bool FlowerGroup::addStepper(Flower *flower) {
    if (containsFlower(flower)) return false;
    flowers[numFlowers] = flower;
    working[numFlowers] = 1;
    workingFlowers++;
    numFlowers++;
    return true;
}

void FlowerGroup::setup() {
    _isRunning = false;
    for (int motorId = 0; motorId < numFlowers; motorId++) {
        flowers[motorId]->setup();
        flowers[motorId]->home();
        if (abs(flowers[motorId]->total_step - 1400) > 400) {
            Serial.println("Stepper " + String(motorId) + " home may not be correct");
        }
        Serial.println("setPosition");
        flowers[motorId]->stepper.setPosition(flowers[motorId]->total_step);
    }
    Serial.println("delay");
    delay(100);
    Serial.println("record");
    recordSensorVal();
    Serial.println("move");
//    moveAbs(0);
    _isRunning = true;
}

void FlowerGroup::forceStop() {
    controller.emergencyStop();
}

void FlowerGroup::softStop() {
    controller.stop();
}

void FlowerGroup::home(int *motors) {
    _isRunning = false;
    for (int i = 0; i < numFlowers; i++) {
        flowers[i]->setup();
        flowers[i]->home();
        if (abs(flowers[i]->total_step - 1400) > 400) {
            flowers[i]->home();
        }
        if (abs(flowers[i]->total_step - 1400) > 400) {
            Serial.print("Stepper ");
            Serial.print(i);
            Serial.println(" home may not correct");
        }
        flowers[i]->stepper.setPosition(flowers[i]->total_step);
        Serial.println("HOMING FINISHED");
    }
    delay(100);
    moveAbs(0);
    _isRunning = true;
}

void FlowerGroup::clear() {

    numFlowers = 0;
}

void FlowerGroup::moveAbs(int target) {
    _isRunning = true;
    // Currently, if not working, set to move to one of the end. 
    Stepper *workingMotors[workingFlowers];
    int count = 0;
    for (int i = 0; i < numFlowers; i++) {
        if (working[i] == 0) {
            if (target == 0) {
                working[i] = 1;
                flowers[i]->stepper.setPosition(0);
                flowers[i]->stepper.setTargetAbs(target);
                finished[i] = 0;
                //workingMotors[_count] = &flowers[i]->stepper;
                //_count ++;
            } else if (finished[i] == 0) {
                //workingMotors[_count] = &flowers[i]->stepper;
                //_count ++;
                flowers[i]->moveUntilStall();
                finished[i] = 1;
                //flowers[i]->stepper.setPosition(0); 
            }
        } else { // if(working[i] == 1)
            flowers[i]->stepper.setTargetAbs(target);
            workingMotors[count] = &flowers[i]->stepper;
            count++;
        }
    }

    if (count == 1) {
        controller.move(*workingMotors[0]);
    } else if (count == 2) {
        controller.move(*workingMotors[0], *workingMotors[1]);
    } else if (count == 3) {
        controller.move(*workingMotors[0], *workingMotors[1], *workingMotors[2]);
    } else if (count == 4) {
        controller.move(*workingMotors[0], *workingMotors[1], *workingMotors[2], *workingMotors[3]);
    } else if (count == 5) {
        controller.move(*workingMotors[0], *workingMotors[1], *workingMotors[2], *workingMotors[3], *workingMotors[4]);
    } else if (count == 6) {
        controller.move(*workingMotors[0], *workingMotors[1], *workingMotors[2], *workingMotors[3], *workingMotors[4],
                        *workingMotors[5]);
    } else if (count == 7) {
        controller.move(*workingMotors[0], *workingMotors[1], *workingMotors[2], *workingMotors[3], *workingMotors[4],
                        *workingMotors[5], *workingMotors[6]);
    } else if (count == 8) {
        controller.move(*workingMotors[0], *workingMotors[1], *workingMotors[2], *workingMotors[3], *workingMotors[4],
                        *workingMotors[5], *workingMotors[6], *workingMotors[7]);
    } else if (count == 9) {
        controller.move(*workingMotors[0], *workingMotors[1], *workingMotors[2], *workingMotors[3], *workingMotors[4],
                        *workingMotors[5], *workingMotors[6], *workingMotors[7], *workingMotors[8]);
    } else if (count == 10) {
        controller.move(*workingMotors[0], *workingMotors[1], *workingMotors[2], *workingMotors[3], *workingMotors[4],
                        *workingMotors[5], *workingMotors[6], *workingMotors[7], *workingMotors[8], *workingMotors[9]);
    }
    _isRunning = false;
}

void FlowerGroup::move(int target) {
    _isRunning = true;
    //BOUND ERROR check
    if (target < 0) {
        target = 0;
    }
    if (target > 100) {
        target = 100;
    }

    Stepper *workingMotors[workingFlowers];
    int count = 0;
    for (int i = 0; i < numFlowers; i++) {
        if (working[i] == 0) {
            if (target == 0) {
                Serial.print("recover Stepper");
                Serial.print(i);
                Serial.println(" finished");
                flowers[i]->stepper.setPosition(0);
                flowers[i]->stepper.setTargetAbs(target);
                working[i] = 1;
                finished[i] = 0;
                workingMotors[count] = &flowers[i]->stepper;
                count++;
            } else if (finished[i] == 0) {
                //workingMotors[_count] = &flowers[i]->stepper;
                //_count ++;
                Serial.print("Stall detected move Stepper");
                Serial.println(i);
                flowers[i]->setRate(300000, 300000 * 5);
                flowers[i]->moveUntilStall();
                flowers[i]->setRate(50000, 50000 * 5);
                finished[i] = 1;
                //flowers[i]->stepper.setPosition(0);74 
            }
        } else {//if(working[i] == 1){
            flowers[i]->stepper.setTargetAbs(flowers[i]->total_step * target / 100);
            workingMotors[count] = &flowers[i]->stepper;
            count++;
        }
    }
    //TODO:Need to fix the variable match in the function
    if (count == 1) {
        controller.move(*workingMotors[0]);
    } else if (count == 2) {
        controller.move(*workingMotors[0], *workingMotors[1]);
    } else if (count == 3) {
        controller.move(*workingMotors[0], *workingMotors[1], *workingMotors[2]);
    } else if (count == 4) {
        controller.move(*workingMotors[0], *workingMotors[1], *workingMotors[2], *workingMotors[3]);
    } else if (count == 5) {
        controller.move(*workingMotors[0], *workingMotors[1], *workingMotors[2], *workingMotors[3], *workingMotors[4]);
    } else if (count == 6) {
        controller.move(*workingMotors[0], *workingMotors[1], *workingMotors[2], *workingMotors[3], *workingMotors[4],
                        *workingMotors[5]);
    } else if (count == 7) {
        controller.move(*workingMotors[0], *workingMotors[1], *workingMotors[2], *workingMotors[3], *workingMotors[4],
                        *workingMotors[5], *workingMotors[6]);
    } else if (count == 8) {
        controller.move(*workingMotors[0], *workingMotors[1], *workingMotors[2], *workingMotors[3], *workingMotors[4],
                        *workingMotors[5], *workingMotors[6], *workingMotors[7]);
    } else if (count == 9) {
        controller.move(*workingMotors[0], *workingMotors[1], *workingMotors[2], *workingMotors[3], *workingMotors[4],
                        *workingMotors[5], *workingMotors[6], *workingMotors[7], *workingMotors[8]);
    } else if (count == 10) {
        controller.move(*workingMotors[0], *workingMotors[1], *workingMotors[2], *workingMotors[3], *workingMotors[4],
                        *workingMotors[5], *workingMotors[6], *workingMotors[7], *workingMotors[8], *workingMotors[9]);
    }
    _isRunning = false;
}


void FlowerGroup::move(int target, int *motors) {
    _isRunning = true;
    //BOUND ERROR check
    if (target < 0) {
        target = 0;
    }
    if (target > 100) {
        target = 100;
    }

    Serial.println(String(target));

    Stepper *workingMotors[workingFlowers];
    int count = 0;
    for (int i = 1; i < motors[0] + 1; i++) {
        if (working[motors[i] - 1] == 0) {
            if (target == 0) {
                Serial.print("recover Stepper");
                Serial.print(motors[i] - 1);
                Serial.println(" finished");
                flowers[motors[i] - 1]->stepper.setPosition(0);
                flowers[motors[i] - 1]->stepper.setTargetAbs(target);
                working[motors[i] - 1] = 1;
                finished[motors[i] - 1] = 0;
                workingMotors[count] = &flowers[motors[i] - 1]->stepper;
                count++;
            } else if (finished[motors[i] - 1] == 0) {
                //workingMotors[_count] = &flowers[i]->stepper;
                //_count ++;
                Serial.print("Stall detected move Stepper ");
                Serial.println(motors[i] - 1);
                flowers[motors[i] - 1]->setRate(300000, 300000 * 5);
                flowers[motors[i] - 1]->moveUntilStall();
                flowers[motors[i] - 1]->setRate(50000, 50000 * 5);
                finished[motors[i] - 1] = 1;
                //flowers[i]->stepper.setPosition(0);74 
            }
        } else {//if(working[i] == 1){
            flowers[motors[i] - 1]->stepper.setTargetAbs(flowers[motors[i] - 1]->total_step * target / 100);
            workingMotors[count] = &flowers[motors[i] - 1]->stepper;
            count++;
        }
    }
    //TODO:Need to fix the variable match in the function
    if (count == 1) {
        controller.move(*workingMotors[0]);
    } else if (count == 2) {
        controller.move(*workingMotors[0], *workingMotors[1]);
    } else if (count == 3) {
        controller.move(*workingMotors[0], *workingMotors[1], *workingMotors[2]);
    } else if (count == 4) {
        controller.move(*workingMotors[0], *workingMotors[1], *workingMotors[2], *workingMotors[3]);
    } else if (count == 5) {
        controller.move(*workingMotors[0], *workingMotors[1], *workingMotors[2], *workingMotors[3], *workingMotors[4]);
    } else if (count == 6) {
        controller.move(*workingMotors[0], *workingMotors[1], *workingMotors[2], *workingMotors[3], *workingMotors[4],
                        *workingMotors[5]);
    } else if (count == 7) {
        controller.move(*workingMotors[0], *workingMotors[1], *workingMotors[2], *workingMotors[3], *workingMotors[4],
                        *workingMotors[5], *workingMotors[6]);
    } else if (count == 8) {
        controller.move(*workingMotors[0], *workingMotors[1], *workingMotors[2], *workingMotors[3], *workingMotors[4],
                        *workingMotors[5], *workingMotors[6], *workingMotors[7]);
    } else if (count == 9) {
        controller.move(*workingMotors[0], *workingMotors[1], *workingMotors[2], *workingMotors[3], *workingMotors[4],
                        *workingMotors[5], *workingMotors[6], *workingMotors[7], *workingMotors[8]);
    } else if (count == 10) {
        controller.move(*workingMotors[0], *workingMotors[1], *workingMotors[2], *workingMotors[3], *workingMotors[4],
                        *workingMotors[5], *workingMotors[6], *workingMotors[7], *workingMotors[8], *workingMotors[9]);
    }
    delete[] motors;
    _isRunning = false;
}

void FlowerGroup::moveInd(int *target, int *motors) {
    while (controller.isRunning()) {
        delay(10);
    }
    _isRunning = true;
    Stepper *workingMotors[workingFlowers];
    int count = 0;
    for (int i = 1; i < motors[0] + 1; i++) {
        if (working[motors[i] - 1] == 0) {
            if (target[i] < 0) {
                target[i] = 0;
            }
            if (target[i] > 100) {
                target[i] = 100;
            }
            if (recoverMethod == 0) {
                if (target[i] == 0) {
                    Serial.print("recover Stepper");
                    Serial.print(motors[i] - 1);
                    Serial.println(" finished");
                    flowers[motors[i] - 1]->stepper.setPosition(0);
                    flowers[motors[i] - 1]->stepper.setTargetAbs(target[i]);
                    working[motors[i] - 1] = 1;
                    finished[motors[i] - 1] = 0;
                    workingMotors[count] = &flowers[motors[i] - 1]->stepper;
                    count++;
                } else if (finished[motors[i] - 1] == 0) {
                    //workingMotors[_count] = &flowers[i]->stepper;
                    //_count ++;
                    Serial.print("Stall detected move Stepper ");
                    Serial.println(motors[i] - 1);
                    flowers[motors[i] - 1]->setRate(300000, 300000 * 5);
                    flowers[motors[i] - 1]->moveUntilStall();
                    flowers[motors[i] - 1]->setRate(50000, 50000 * 5);
                    finished[motors[i] - 1] = 1;
                    //flowers[i]->stepper.setPosition(0);74 
                }
            } else if (recoverMethod == 1) {
                flowers[motors[i] - 1]->setRate(300000, 300000 * 5);
                flowers[motors[i] - 1]->moveUntilStall();
                flowers[motors[i] - 1]->setRate(50000, 50000 * 5);
                flowers[motors[i] - 1]->stepper.setPosition(0);
                flowers[motors[i] - 1]->stepper.setTargetAbs(target[i]);
                working[motors[i] - 1] = 1;
                finished[motors[i] - 1] = 0;
                workingMotors[count] = &flowers[motors[i] - 1]->stepper;
                count++;
            }


        } else {//if(working[i] == 1){
            flowers[motors[i] - 1]->stepper.setTargetAbs(flowers[motors[i] - 1]->total_step * target[i] / 100);
            workingMotors[count] = &flowers[motors[i] - 1]->stepper;
            count++;
        }
    }
    //TODO:Need to fix the variable match in the function
    if (count == 1) {
        controller.moveAsync(*workingMotors[0]);
    } else if (count == 2) {
        controller.moveAsync(*workingMotors[0], *workingMotors[1]);
    } else if (count == 3) {
        controller.moveAsync(*workingMotors[0], *workingMotors[1], *workingMotors[2]);
    } else if (count == 4) {
        controller.moveAsync(*workingMotors[0], *workingMotors[1], *workingMotors[2], *workingMotors[3]);
    } else if (count == 5) {
        controller.moveAsync(*workingMotors[0], *workingMotors[1], *workingMotors[2], *workingMotors[3],
                             *workingMotors[4]);
    } else if (count == 6) {
        controller.moveAsync(*workingMotors[0], *workingMotors[1], *workingMotors[2], *workingMotors[3],
                             *workingMotors[4], *workingMotors[5]);
    } else if (count == 7) {
        controller.moveAsync(*workingMotors[0], *workingMotors[1], *workingMotors[2], *workingMotors[3],
                             *workingMotors[4], *workingMotors[5], *workingMotors[6]);
    } else if (count == 8) {
        controller.moveAsync(*workingMotors[0], *workingMotors[1], *workingMotors[2], *workingMotors[3],
                             *workingMotors[4], *workingMotors[5], *workingMotors[6], *workingMotors[7]);
    } else if (count == 9) {
        controller.moveAsync(*workingMotors[0], *workingMotors[1], *workingMotors[2], *workingMotors[3],
                             *workingMotors[4], *workingMotors[5], *workingMotors[6], *workingMotors[7],
                             *workingMotors[8]);
    } else if (count == 10) {
        controller.moveAsync(*workingMotors[0], *workingMotors[1], *workingMotors[2], *workingMotors[3],
                             *workingMotors[4], *workingMotors[5], *workingMotors[6], *workingMotors[7],
                             *workingMotors[8], *workingMotors[9]);
    }
    delete[] motors;
    _isRunning = false;
}

bool FlowerGroup::containsFlower(Flower *f) {
    for (int i = 0; i < numFlowers; i++) {
        Flower *currentFlower = flowers[i];
        if (*currentFlower == *f) return true;
    }
    return false;
}
#include "Arduino.h"
#include "FlowerGroup.h"
#include "Flower.h"
#include "TeensyStep.h"

IntervalTimer myTimer;
Flower* flowers[10];
volatile int totalFlower;
int workingFlowers;

int working[10]= {0,0,0,0,0,0,0,0,0,0};
int finished[10] ={0,0,0,0,0,0,0,0,0,0};
StepControl controller;
volatile float step2SensorVal[10][2000];
//stall detection variables
volatile int _count[10]={0,0,0,0,0,0,0,0,0,0};
volatile float _sum[10]={0,0,0,0,0,0,0,0,0,0};
volatile float _lastsum[10]={0,0,0,0,0,0,0,0,0,0};
volatile bool _isrunning ;


bool FlowerGroup::isRunning(){
    return _isrunning;
}

FlowerGroup::FlowerGroup(){
    totalFlower = 0;
    workingFlowers = 0;
    _isrunning = false;
    myTimer.begin(checkStall, 1000); 
}

void FlowerGroup::recordSensorVal(){
    Serial.println("SENSOR VALUE RECORDING");
    for(int i=0; i<totalFlower; i++){
        Serial.println(flowers[i]->total_step);
        for (int j=0; j<flowers[i]->total_step;j++){
            flowers[i]->stepper.setTargetAbs(j);
            controller.move(*&flowers[i]->stepper);
            step2SensorVal[i][j] = analogRead(flowers[i]->_sensorpin);
            delay(5);
            
        }
    }
    Serial.println("SENSOR VALUE FINISHED");
}

float FlowerGroup::getSensorVal(Flower* f){
    return analogRead(f->_sensorpin);
}

void FlowerGroup::checkStall(){
    if(_isrunning){
        for(int i=0; i<totalFlower; i++){
            if(working[i] == 1){
                float tmp = analogRead(flowers[i]->_sensorpin);
                if(_count[i] <12){
                    _count[i] += 1;
                    _sum[i] += tmp;
                }else{
                    if(_lastsum[i] == 0){
                        _lastsum[i] = _sum[i];
                    }else{
                        if(abs(_lastsum[i] - _sum[i])< 8 && abs(tmp - step2SensorVal[i][flowers[i]->stepper.getPosition()]) > 300 
                                && abs(flowers[i]->stepper.getPosition() - flowers[i]->total_step) > 200 && flowers[i]->stepper.getPosition() > 200){
                            Serial.println("Stall Detected");
                            Serial.print(tmp);
                            Serial.print(" ");
                            Serial.print(abs(_lastsum[i] - _sum[i]));
                            Serial.print(" ");
                            Serial.print(flowers[i]->stepper.getPosition());
                            Serial.print(" ");
                            Serial.println(step2SensorVal[i][flowers[i]->stepper.getPosition()]);
                            working[i] = 0;
                        }
                        _lastsum[i] = _sum[i];
                        _count[i] = 0;
                        _sum[i] = 0;
                    }
                }
            }
            
        }
    }
}

void FlowerGroup::addStepper(Flower& flower){
    flowers[totalFlower] = &flower;
    working[totalFlower] = 1;
    workingFlowers++;
    totalFlower++;
}

void FlowerGroup::setup(){
    for(int i=0; i<totalFlower; i++){
        flowers[i]->setup();
        flowers[i]->home();
        if(abs(flowers[i]->total_step - 1400) > 400){
            flowers[i]->home();
        }
        if(abs(flowers[i]->total_step - 1400) > 400){
            Serial.print("Stepper ");
            Serial.print(i);
            Serial.println(" home may not correct");
        }
        flowers[i]->stepper.setPosition(flowers[i]->total_step);
        Serial.println("HOMING FINISHED");
    }
    delay(100);
    recordSensorVal();
    moveAbs(0);
    _isrunning = true;
}

void FlowerGroup::clear(){

    totalFlower = 0;
}

void FlowerGroup::moveAbs(int target){
    // Currently, if not working, set to move to one of the end. 
    Stepper *workingMotors[workingFlowers];
    int count = 0;
    for(int i=0; i<totalFlower; i++){
        if(working[i] == 0){
            if(target == 0){
                working[i] = 1;
                flowers[i]->stepper.setPosition(0);
                flowers[i]->stepper.setTargetAbs(target);
                finished[i] = 0;
                //workingMotors[count] = &flowers[i]->stepper;
                //count ++;
            }else if(finished[i] == 0){
                //workingMotors[count] = &flowers[i]->stepper;
                //count ++;
                flowers[i]->moveUntilStall();
                finished[i] = 1;
                //flowers[i]->stepper.setPosition(0); 
            }
        }
        else{ // if(working[i] == 1)
            flowers[i]->stepper.setTargetAbs(target);
            workingMotors[count] = &flowers[i]->stepper;
            count ++;
        }
    }
    
    if(count == 1){
        controller.move(*workingMotors[0]);
    }else if(count == 2){
        controller.move(*workingMotors[0],*workingMotors[1]);
    }else if (count == 3){
        controller.move(*workingMotors[0],*workingMotors[1],*workingMotors[2]);
    }else if (count == 4){
        controller.move(*workingMotors[0],*workingMotors[1],*workingMotors[2],*workingMotors[3]);
    }else if (count == 5){
        controller.move(*workingMotors[0],*workingMotors[1],*workingMotors[2],*workingMotors[3],*workingMotors[4]);
    }else if (count == 6){
        controller.move(*workingMotors[0],*workingMotors[1],*workingMotors[2],*workingMotors[3],*workingMotors[4],*workingMotors[5]);
    }else if (count ==7){
        controller.move(*workingMotors[0],*workingMotors[1],*workingMotors[2],*workingMotors[3],*workingMotors[4],*workingMotors[5],*workingMotors[6]);
    }else if (count == 8){
        controller.move(*workingMotors[0],*workingMotors[1],*workingMotors[2],*workingMotors[3],*workingMotors[4],*workingMotors[5],*workingMotors[6],*workingMotors[7]);
    }else if (count ==9){
        controller.move(*workingMotors[0],*workingMotors[1],*workingMotors[2],*workingMotors[3],*workingMotors[4],*workingMotors[5],*workingMotors[6],*workingMotors[7],*workingMotors[8]);
    }else if (count == 10){
        controller.move(*workingMotors[0],*workingMotors[1],*workingMotors[2],*workingMotors[3],*workingMotors[4],*workingMotors[5],*workingMotors[6],*workingMotors[7],*workingMotors[8],*workingMotors[9]);
    }

    
}

void FlowerGroup::move(int target){
    //BOUND ERROR check
    if(target<0){
        target = 0;
    }
    if(target > 100){
        target = 100;
    }

    Stepper *workingMotors[workingFlowers];
    int count = 0;
    for(int i=0; i<totalFlower; i++){
        if(working[i] == 0){
            if(target == 0){
                Serial.print("recover Stepper");
                Serial.print(i);
                Serial.println(" finished");
                flowers[i]->stepper.setPosition(0);
                flowers[i]->stepper.setTargetAbs(target);
                working[i] = 1;
                finished[i] = 0;
                workingMotors[count] = &flowers[i]->stepper;
                count ++;
            }else if(finished[i] == 0){
                //workingMotors[count] = &flowers[i]->stepper;
                //count ++;
                Serial.print("Stall detected move Stepper");
                Serial.println(i);
                flowers[i]->setRate(300000);
                flowers[i]->moveUntilStall();
                flowers[i]->setRate(50000);
                finished[i] = 1;
                //flowers[i]->stepper.setPosition(0);74 
            }
        }else{//if(working[i] == 1){
            flowers[i]->stepper.setTargetAbs(flowers[i]->total_step * target/100);
            workingMotors[count] = &flowers[i]->stepper;
            count ++;
        }
    }
    //TODO:Need to fix the variable match in the function
    if(count == 1){
        controller.move(*workingMotors[0]);
    }else if(count == 2){
        controller.move(*workingMotors[0],*workingMotors[1]);
    }else if (count == 3){
        controller.move(*workingMotors[0],*workingMotors[1],*workingMotors[2]);
    }else if (count == 4){
        controller.move(*workingMotors[0],*workingMotors[1],*workingMotors[2],*workingMotors[3]);
    }else if (count == 5){
        controller.move(*workingMotors[0],*workingMotors[1],*workingMotors[2],*workingMotors[3],*workingMotors[4]);
    }else if (count == 6){
        controller.move(*workingMotors[0],*workingMotors[1],*workingMotors[2],*workingMotors[3],*workingMotors[4],*workingMotors[5]);
    }else if (count ==7){
        controller.move(*workingMotors[0],*workingMotors[1],*workingMotors[2],*workingMotors[3],*workingMotors[4],*workingMotors[5],*workingMotors[6]);
    }else if (count == 8){
        controller.move(*workingMotors[0],*workingMotors[1],*workingMotors[2],*workingMotors[3],*workingMotors[4],*workingMotors[5],*workingMotors[6],*workingMotors[7]);
    }else if (count ==9){
        controller.move(*workingMotors[0],*workingMotors[1],*workingMotors[2],*workingMotors[3],*workingMotors[4],*workingMotors[5],*workingMotors[6],*workingMotors[7],*workingMotors[8]);
    }else if (count == 10){
        controller.move(*workingMotors[0],*workingMotors[1],*workingMotors[2],*workingMotors[3],*workingMotors[4],*workingMotors[5],*workingMotors[6],*workingMotors[7],*workingMotors[8],*workingMotors[9]);
    }

}
#include "Arduino.h"
#include "FlowerGroup.h"
#include "Flower.h"
#include "TeensyStep.h"

FlowerGroup::FlowerGroup(){
    totalFlower = 0;
    workingFlowers = 0;
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
        flowers[i]->stepper.setPosition(flowers[i]->total_step);
        
    }
}

void FlowerGroup::clear(){

    totalFlower = 0;
}

void FlowerGroup::moveAbs(int target){
    Stepper *workingMotors[workingFlowers];
    int count = 0;
    for(int i=0; i<totalFlower; i++){
        
        if(working[i] == 1){
            Serial.println(flowers[i]->stepper.getPosition());
            flowers[i]->stepper.setTargetAbs(target);
            workingMotors[count] = &flowers[i]->stepper;
            count ++;
        }
    }
    controller.move(*workingMotors[0], *workingMotors[1]);
}

void FlowerGroup::move(int target){
    Stepper *workingMotors[workingFlowers];
    int count = 0;
    for(int i=0; i<totalFlower; i++){
        
        if(working[i] == 1){
            flowers[i]->stepper.setTargetAbs(flowers[i]->total_step * target/100);
            workingMotors[count] = &flowers[i]->stepper;
            count ++;
        }
    }
    //TODO:Need to fix the variable match in the function
    controller.move(*workingMotors[0], *workingMotors[1]);
}
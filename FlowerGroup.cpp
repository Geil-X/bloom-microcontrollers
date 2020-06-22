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
    Serial.println(totalFlower);
}

void FlowerGroup::setup(){
    Serial.println(totalFlower);
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
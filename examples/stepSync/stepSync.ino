#include "Flower.h"
#include "TeensyStep.h"

Flower flower1(2,3, A9);
Flower flower2(7,8, A8); 
StepControl controller;
void setup()
{
    flower1.setup();
    flower1.home();
    delay(1000);
    flower2.setup();
    flower2.home();
    delay(1000);
    
    flower1.stepper.setPosition(flower1.total_step);
    flower2.stepper.setPosition(flower2.total_step);
    

    flower1.stepper.setTargetAbs(0);
    flower2.stepper.setTargetAbs(0);
    controller.move(flower1.stepper,flower2.stepper);
    
    
    delay(1000);
    
}

void loop()
{

  for(int i=0;i<100;i+=10){
    flower1.stepper.setTargetAbs(flower1.total_step*i/100);
    flower2.stepper.setTargetAbs(flower2.total_step*i/100);
    controller.move(flower1.stepper,flower2.stepper);
    delay(1000);
  }
    
}

#include "Flower.h"
#include "TeensyStep.h"
#include "FlowerGroup.h"

Flower flower1(2,3, A9);
Flower flower2(7,8, A8); 
FlowerGroup group;
void setup()
{
    group.addStepper(flower1);
    group.addStepper(flower2);
    group.setup();

  
    group.moveAbs(0);
    
    
    delay(1000);
    
}

void loop()
{

  for(int i=0;i<100;i+=10){
    group.move(i);
  }
    
}
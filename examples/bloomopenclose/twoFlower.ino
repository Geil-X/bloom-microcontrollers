
#include "Flower.h"
Flower flower1(2,3,4,5,6, A9);
Flower flower2(7,8,9,10,11, A8);
int count = 0;
 bool startCount = false;
void setup()
{
  flower1.setup();
  flower1.home();
  flower1.setRate(30);
  flower2.setup();
  flower2.home();
  flower2.setRate(30);



}

void loop()
{
  for(int i=0;i<100;i+=10){
    flower1.open(i);
    delay(100);
  }
  for(int i=0;i<100;i+=10){
    flower2.open(i);
    delay(100);
  }
  
  
}
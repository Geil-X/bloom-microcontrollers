
#include "Flower.h"
#include "TeensyThreads.h"

volatile Flower flower1(2,3,4,5,6, A9);
volatile Flower flower2(7,8,9,10,11, A8);
int count = 0;
 bool startCount = false;
void setup()
{
  flower1.setup();
  flower1.home();
  flower1.setRate(30);
  
  //delay(1000);
  //Serial.println("Stop");
  //flower.setRate(10);
  flower1.close();
  flower1.open();

}

void loop()
{
 
}

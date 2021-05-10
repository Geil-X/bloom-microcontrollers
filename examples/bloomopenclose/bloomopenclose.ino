
#include "Flower.h"
Flower flower(2,3,4,5,6, A9);
int count = 0;
 bool startCount = false;
void setup()
{
  flower.setup();
  flower.home();
  flower.setRate(30);
  
  //delay(1000);
  //Serial.println("Stop");
  //flower.setRate(10);
  flower.close();
  flower.open();

}

void loop()
{
  flower.open();
  flower.close();
  for(int i=0;i<100;i+=10){
    flower.open(i);
  }
  //flower.continueUntilStall();
  delay(1000);
  flower.slowlyOpen();
flower.slowlyClose();
delay(1000);
}
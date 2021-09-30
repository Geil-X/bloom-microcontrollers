#include "Arduino.h"
#include <AH_EasyDriver.h>

int dir = 1;
double _sum = 0;
double _lastSum = 0;
int _count = 0;
AH_EasyDriver stepper(200,2,3,4,5,6); 
bool ismove=true;
void setup() {
Serial.begin (115200);
stepper.sleepOFF();
    stepper.setMicrostepping(3);
    stepper.setSpeedRPM(20); 

}

void loop(){
  if(_count > 20){
    Serial.print("Sum Diff");
    Serial.println(abs(_sum / _count - _lastSum / _count));
    if(abs(_sum / _count - _lastSum / _count) < 3){
      Serial.print("Stop    ");
      //ismove = false;
      if(dir == 1){
        dir = -1;
      }else{
        dir = 1;
      }
      Serial.println(dir);
      stepper.move(dir*100);
      delay(10);
    }
      _count = 0;
      _lastSum = _sum;
      _sum = analogRead(A9);
  }else{
    int temp = analogRead(A9);
   //#Serial.println(temp);
    _sum += temp;
      _count += 1;
  }
  
  
if(ismove){
    stepper.move(dir);
  }
  
  
  
  delay(8);



}

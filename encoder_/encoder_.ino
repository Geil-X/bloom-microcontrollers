

#include "Arduino.h"
#include <AH_EasyDriver.h>

int sensorvalue=0;
int dir = 1;
double sum = 0;
double lastsum = 0;
int count = 0;
AH_EasyDriver stepper(200,2,3,4,5,6); 
bool ismove=true;
void setup() {
Serial.begin (115200);
stepper.sleepOFF();
    stepper.setMicrostepping(3);
    stepper.setSpeedRPM(20); 

}

void loop(){
  if(count >20){
    Serial.print("Sum Diff");
    Serial.println(abs(sum / count - lastsum / count));
    if(abs(sum / count - lastsum / count) < 3){
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
    count = 0;
    lastsum = sum;
    sum = analogRead(A9);
  }else{
    int temp = analogRead(A9);
   //#Serial.println(temp);
    sum += temp;
    count += 1;
  }
  
  
if(ismove){
    stepper.move(dir);
  }
  
  
  
  delay(8);



}

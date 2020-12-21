#include "Flower.h"
#include "TeensyStep.h"
#include "FlowerGroup.h"


FlowerGroup group;

bool debug = false;
void setup()
{
  
}

void loop()
{
   if (Serial.available() > 0) {
      String commandValue = Serial.readStringUntil('\n').trim().toUpperCase();
      int spaceIndex = commandValue.indexOf(' ');
      String commandName;
      if (spaceIndex == -1){
        commandName = commandValue;
      }else{
        commandName = commandValue.substring(0, spaceIndex);
      }
      if (commandName == "NEWSTEPPER"){
          int secondSpaceIndex = commandValue.indexOf(' ', spaceIndex + 1);
          String commandValues = commandValue.substring(spaceIndex + 1);
          int thirdSpaceIndex = commandValue.indexOf(' ', secondSpaceIndex + 1);
          String commandValues2 = commandValue.substring(secondSpaceIndex + 1);
          String commandValues3 = commandValue.substring(thirdSpaceIndex + 1);
          addStepper(commandValues.toInt(), commandValues2.toInt(),commandValues3.toInt());
      }else if (commandName == "SETUP"){
          Serial.println("Start Setup");
          group.setup();
          Serial.println("Finish Setup");
          //TODO: Add function for thread 
          group.setThrehold(0,8,40,40);
          group.setThrehold(1,8,40,40);
      }else if (commandName == "HOME"){
          int moveValue = commandValue.substring(spaceIndex + 1).toInt();
          if (moveValue == "ALL"){
            group.setup();
          }else{
            group.home(parseNumbers(moveValue));
          }
        }else if (commandName == "OPEN"){
          int secondSpaceIndex = commandValue.indexOf(' ', spaceIndex + 1);
          int moveValue = commandValue.substring(spaceIndex + 1).toInt();
          String commandValues2 = commandValue.substring(secondSpaceIndex + 1);
          if (commandValues2 == "ALL"){
            group.move(moveValue);
          }else{
            group.move(moveValue, parseNumbers(commandValues2));
          }
      }else if (commandName == "OPENIND"){
          int secondSpaceIndex = commandValue.indexOf(' ', spaceIndex + 1);
          String moveValue = commandValue.substring(spaceIndex + 1);
          String commandValues2 = commandValue.substring(secondSpaceIndex + 1);
          group.moveInd(parseNumbers(moveValue), parseNumbers(commandValues2));
      }else if (commandName == "STAT"){
        group.stat();
      }else if (commandName == "DEBUG"){
        debug = !debug;
        Serial.println("Debug Mode :" + String(debug));
      }else if (commandName == "SOFTSTOP"){
        group.softStop();
      }else if (commandName == "FORCESTOP"){
        group.forceStop();
      }else if (commandName == "RESET"){
        group.reset();
      }
    }
}

void addStepper(int port1, int port2, int sensorPin){
  Serial.println("Flower "+String(port1)+" "+String(port2)+" "+String(sensorPin)+" Created.");
  //Flower* flower1 = new Flower(port1, port2, sensorPin);
  group.addStepper(new Flower(port1, port2, sensorPin)); 
  group.stat();
  Serial.print("Flower Added to the group. Total number of Flower:");
  Serial.println(group.numFlower());
}


int* parseNumbers(String s){
  int count = 0;
  for (int i = 0; i < s.length(); i++)
    if (s[i] == ',') count++;
  int *number = new int[count+2];
  number[0] = count + 1;
  String rest = s;
  for (int i=1; i< count+1;i++){
    int commaIndex = rest.indexOf(',');
    int value = rest.substring(0, commaIndex).toInt();
    number[i] = value;
    rest = rest.substring(commaIndex+1);
  }
  int value = rest.toInt();
  number[count+1] = value;
  return number;
}


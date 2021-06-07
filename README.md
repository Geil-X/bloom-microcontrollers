# Flower
Flower C++ Library for individual controls


# Setup

* Install the [Platform IO](https://platformio.org/install/cli) command line interface.
* If using the CLion development IDE you can install the [Platform IO Plugin](https://plugins.jetbrains.com/plugin/13922-platformio-for-clion).
    * You can learn more about CLion and Platform IO through this [tutorial](https://blog.jetbrains.com/clion/2020/08/arduino-from-hobby-to-prof-p1/).


# Installation Steps (In progress):
- Download this repo as Zip
- Install the Zip as library in Arduino / Teenarduino program

```
#include "Flower.h"
Flower flower(2,3,4,5,6,A9);  <- DIR_PIN, STEP_PIN, MS1, MS2, SLP, Sensor Pin

void setup()
{
  flower.setup();    <-  Initial flower
  flower.home();     <- Auto Home
  flower.setRate(30);   <- Set rate to 30rpm
  
  flower.close();   <- move to Close position
  flower.open();   <- move to Open position

}

void loop()
{
  flower.open();
  flower.close();
  for(int i=0;i<100;i+=10){
    flower.open(i);    <- move to Open position. certain angle (in percentage)
  }
  //flower.continueUntilStall();
  delay(1000);
  flower.slowlyOpen();    <- rate change open
flower.slowlyClose();     <- rate change close
delay(1000);
}

```

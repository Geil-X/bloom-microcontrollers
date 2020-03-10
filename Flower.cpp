/*
  Flower.cpp - Library for single flower node.
  Created by Ramon Qu.
*/

#include "Arduino.h"
#include "SPI.h"
#include "Flower.h"
#include <AH_EasyDriver.h>


AH_EasyDriver stepper(200,2,3,4,5,6); 

// scilent driver
int state = HIGH;      // the current state of the output pin
int reading;           // the current reading from the input pin
int previous = LOW;    // the previous reading from the input pin

int EN_PIN = 7; //enable (CFG6)
int DIR_PIN = 8; //direction
int STEP_PIN  = 9; //step

int CS_PIN  = 10; //chip select
int MOSI_PIN = 11; //SDI/MOSI (ICSP: 4, Uno: 11, Mega: 51)
int MISO_PIN =12; //SDO/MISO (ICSP: 1, Uno: 12, Mega: 50)
int SCK_PIN = 13; //CLK/SCK  (ICSP: 3, Uno: 13, Mega: 52)

int bandrate = 9600;

//TMC2130 registers
#define WRITE_FLAG     (1<<7) //write flag
#define READ_FLAG      (0<<7) //read flag
#define REG_GCONF      0x00
#define REG_GSTAT      0x01
#define REG_IHOLD_IRUN 0x10
#define REG_CHOPCONF   0x6C
#define REG_COOLCONF   0x6D
#define REG_DCCTRL     0x6E
#define REG_DRVSTATUS  0x6F

long current_step = 0;
static long total_step = 1600;


uint8_t tmc_write(uint8_t cmd, uint32_t data)
{
  uint8_t s;

  digitalWrite(CS_PIN, LOW);

  s = SPI.transfer(cmd);
  SPI.transfer((data>>24UL)&0xFF)&0xFF;
  SPI.transfer((data>>16UL)&0xFF)&0xFF;
  SPI.transfer((data>> 8UL)&0xFF)&0xFF;
  SPI.transfer((data>> 0UL)&0xFF)&0xFF;

  digitalWrite(CS_PIN, HIGH);

  return s;
}

uint8_t tmc_read(uint8_t cmd, uint32_t *data)
{
  uint8_t s;

  tmc_write(cmd, 0UL); //set read address

  digitalWrite(CS_PIN, LOW);

  s = SPI.transfer(cmd);
  *data  = SPI.transfer(0x00)&0xFF;
  *data <<=8;
  *data |= SPI.transfer(0x00)&0xFF;
  *data <<=8;
  *data |= SPI.transfer(0x00)&0xFF;
  *data <<=8;
  *data |= SPI.transfer(0x00)&0xFF;

  digitalWrite(CS_PIN, HIGH);

  return s;
}


Flower::Flower(int EN_PIN,int DIR_PIN,int STEP_PIN,int CS_PIN,int MOSI_PIN,int MISO_PIN,int SCK_PIN)
{
  _driver = 0;
  _enpin = EN_PIN;
  _dirpin = DIR_PIN;
  _steppin = STEP_PIN;
  _cspin = CS_PIN;
  _mosipin = MOSI_PIN;
  _misopin = MISO_PIN;
  _sckpin = SCK_PIN;
  _dir = 1;
  _rate=10;
  _function = false;

}

Flower::Flower(int DIR_PIN,int STEP_PIN,int MS1,int MS2,int SLP){
  _driver = 1;
  _enpin = EN_PIN;
  _dirpin = DIR_PIN;
  _steppin = STEP_PIN;
  _ms1pin = MS1;
  _cspin = CS_PIN;
  _ms2pin = MS2;
  _slppin = SLP;
  
  _dir = 1;
  _rate=10;
  _function = false;
}


void Flower::setup()
{
  if(_driver ==0 ){
    //set pins
    pinMode(EN_PIN, OUTPUT);
    digitalWrite(EN_PIN, HIGH); //deactivate driver (LOW active)
    pinMode(DIR_PIN, OUTPUT);
    digitalWrite(DIR_PIN, _dir); //LOW or HIGH
    pinMode(STEP_PIN, OUTPUT);
    digitalWrite(STEP_PIN, LOW);

    pinMode(CS_PIN, OUTPUT);
    digitalWrite(CS_PIN, HIGH);
    pinMode(MOSI_PIN, OUTPUT);
    digitalWrite(MOSI_PIN, LOW);
    pinMode(MISO_PIN, INPUT);
    digitalWrite(MISO_PIN, HIGH);
    pinMode(SCK_PIN, OUTPUT);
    digitalWrite(SCK_PIN, LOW);

    //init serial port
    Serial.begin(bandrate); //init serial port and set baudrate
    while(!Serial); //wait for serial port to connect (needed for Leonardo only)
    Serial.println("\nStart...");

    //init SPI
    SPI.begin();
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE3));

    //set TMC2130 config
    tmc_write(WRITE_FLAG|REG_GCONF,      0x00000001UL); //voltage on AIN is current reference
    tmc_write(WRITE_FLAG|REG_IHOLD_IRUN, 0x00001010UL); //IHOLD=0x10, IRUN=0x10
    tmc_write(WRITE_FLAG|REG_CHOPCONF,   0x00008008UL); //native 256 microsteps, MRES=0, TBL=1=24, TOFF=8

    //TMC2130 outputs on (LOW active)
    digitalWrite(_enpin, LOW); 
    _lasttime=0;
  }
  else{
    stepper.sleepOFF();
    stepper.setMicrostepping(3);
    stepper.setSpeedRPM(_rate); 
    //stepper(200,2,3,4,5,6);    // init w/o "enable" and "reset" functions
  }
  
}
int test(int x){
  return x*x;
}

void Flower::step()
{
  if(_driver==0){
    if(_function){
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(test(current_step));
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(test(current_step));
    }else{
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(_rate);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(_rate);
    }
      if(_dir == 0){
        current_step += 1;
      }else{
        current_step -= 1;
      }
  }else{
    if(_function){
      if(_dir == 0){
          stepper.move(-1);
          current_step += 1;
        }else{
          stepper.move(1);
          current_step -= 1;
        }
    }else{
        if(_dir == 0){
          stepper.move(-1);
          current_step += 1;
        }else{
          stepper.move(1);
          current_step -= 1;
        }
    }
  }
}

void Flower::slowlyOpen(){
  setDir(true);
  while (current_step > 0){
    setRate((int)current_step / 1600.0 * 90.0 + 10);
    step();
  }
}

void Flower::slowlyClose(){
  setDir(false);
  while (current_step < total_step){
    setRate((int)current_step / 1600.0 * 90.0 + 10);
    step();
  }
}

void Flower::enableFunction(bool enable){
  if(enable){
    _function = true; 
  }else{
    _function = false;
  }
}


void Flower::reverse()
{
  _dir = 1 - _dir;
  Serial.print("Direction: ");
  Serial.println(_dir);
  digitalWrite(_dirpin, _dir);
}

void Flower::setRate(int rate)
{
  if(_driver == 0){
    _rate = rate;
  }else{
    stepper.setSpeedRPM(rate); 
  }
}
void Flower::home()
{
  // Close to the end.
    digitalWrite(DIR_PIN, 1);

    static uint32_t last_time=0;
    int reading = 0;

    while (reading != 1)
    {
      uint32_t ms = millis();
      uint32_t data;
      uint8_t s;
      if((ms-last_time) > 1500 ) //run every 1s
      {
        last_time = ms;

        //show REG_GSTAT
        s = tmc_read(REG_GSTAT, &data);
        Serial.print("GSTAT:     0x0");
        Serial.print(data, HEX);
        Serial.print("\t - ");
        Serial.print("Status: 0x");
        Serial.print(s, HEX);
        if(s & 0x01) Serial.print(" reset");
        if(s & 0x02) Serial.print(" error");
        if(s & 0x04) Serial.print(" sg2");
        if(s & 0x08) Serial.print(" standstill");
        Serial.println(" ");

        //show REG_DRVSTATUS
        s = tmc_read(REG_DRVSTATUS, &data);
        Serial.print("DRVSTATUS: 0x");
        Serial.print(data, HEX);
        Serial.print("\t - ");
        Serial.print("Status: 0x");
        Serial.print(s, HEX);
        if(s & 0x01) Serial.print(" reset");
        if(s & 0x02) Serial.print(" error");
        if(s & 0x04) Serial.print(" sg2");
        if(s & 0x08) Serial.print(" standstill");
        Serial.println(" ");


        reading = digitalRead(DIR_PIN);
      }
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(10);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(10);
      //s = tmc_read(REG_DRVSTATUS, &data);
      //if(s & 0x08) Serial.print(" standstill");
    }
}

void Flower::continueUntilStall(){
  
}

void Flower::open(){
  //Going down
  setDir(true);
  while (current_step > 0){
    step();
  }
}

void Flower::close(){
  //Going up
  setDir(false);
  if(_driver == 0){
    while (current_step < total_step){
      step();
    }
  }else{
    while (current_step < total_step){
      step();
    }
  }
  
  
}

void Flower::report(){
  uint32_t ms = millis();
    uint32_t data;
    uint8_t s;
    //show REG_GSTAT
      s = tmc_read(REG_GSTAT, &data);
      Serial.print("GSTAT:     0x0");
      Serial.print(data, HEX);
      Serial.print("\t - ");
      Serial.print("Status: 0x");
      Serial.print(s, HEX);
      if(s & 0x01) Serial.print(" reset");
      if(s & 0x02) Serial.print(" error");
      if(s & 0x04) Serial.print(" sg2");
      if(s & 0x08) Serial.print(" standstill");
      Serial.println(" ");

      //show REG_DRVSTATUS
      s = tmc_read(REG_DRVSTATUS, &data);
      Serial.print("DRVSTATUS: 0x");
      Serial.print(data, HEX);
      Serial.print("\t - ");
      Serial.print("Status: 0x");
      Serial.print(s, HEX);
      if(s & 0x01) Serial.print(" reset");
      if(s & 0x02) Serial.print(" error");
      if(s & 0x04) Serial.print(" sg2");
      if(s & 0x08) Serial.print(" standstill");
      Serial.println(" ");

      Serial.print("rate: ");
      Serial.print(_rate);
}

void Flower::open(int percentage){
  long target = (long)total_step * percentage / 100;
  if(current_step < target){
    setDir(false);
    while (current_step < target){
      step();
    }
  }else{
    setDir(true);
    while (current_step > target){
      step();
    }
  }

}

void Flower::setDir(bool open){
  if (open){
    _dir = 1;
  }else{
    _dir = 0;
  }
  if(_driver==0){
    digitalWrite(DIR_PIN, _dir);
  }
    
}
  
/*
  Flower.h - Library for controlling single flower code.
  Created by Ramon Qu.
*/
#ifndef Flower_h
#define Flower_h

#include "Arduino.h"

class Flower
{
  public:
    Flower(int EN_PIN,int DIR_PIN,int STEP_PIN,int CS_PIN,int MOSI_PIN,int MISO_PIN,int SCK_PIN);
    Flower(int DIR_PIN,int STEP_PIN,int MS1,int MS2,int SLP);
    void setup();
    void step();
    void reverse();
    void setRate(int rate);
    void home();
    void open();
    void open(int percentage);
    void close();
    void setDir(bool open);
    void report();
    void enableFunction(bool enable);
    void slowlyOpen();
    void slowlyClose();
  private:
    int _enpin;
    int _dirpin;
    int _steppin;
    int _cspin;
    int _mosipin;
    int _misopin;
    int _sckpin;
    int _ms1pin;
    int _ms2pin;
    int _slppin;
    int _dir;
    int _driver;
    float _rate;
    uint32_t _lasttime;
    bool _function;
    
};

#endif
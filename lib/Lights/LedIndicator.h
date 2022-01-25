#ifndef FLOWER_LEDINDICATOR_H
#define FLOWER_LEDINDICATOR_H

#include <Arduino.h>

class LedIndicator {

public:
    explicit LedIndicator(uint8_t pin);
    void blink(unsigned long on_time_ms, unsigned long off_time_ms);
    void blinkBlocking(unsigned long on_time_ms, unsigned long off_time_ms, int count);
    void on();
    void off();
    // Return true when one blink cycle was completed
    bool update();

private:
    uint8_t pin;
    bool pin_state = LOW;
    unsigned long off_time = 0;
    unsigned long on_time = 0;
    unsigned long start_time = 0;

};


#endif //FLOWER_LEDINDICATOR_H

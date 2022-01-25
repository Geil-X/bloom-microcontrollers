#include "Interrupt.h"
#include <Arduino.h>

/// This function works off of timer1. Create the `ISR(TIMER1_COMPA_vect)`
/// function in the code to use this function.
///
/// @param usecs The timer interrupt period in microseconds
/// @see https://groups.google.com/g/accelstepper/c/FIct-aiBk3o
/// @author James Conway (jimc)
bool setUpInterrupts(const int usecs) {
    // initialize Timer1
    cli(); // disable global interrupts
    TCCR1A = 0; // set entire TCCR1A register to 0
    TCCR1B = 0; // same for TCCR1B

    // set compare match register to desired timer count (1ms):
    // ATmega328 with a 16MHz clock, clk/8
    // (# timer counts + 1) = (target time) / (timer resolution)
    // = .0001s / 6.25e-8 s * 8
    // = 200
    const float targetSecs = ((float) usecs) / 1e6f;
    const float timerSteps = 6.25e-8; // 1/16MHz
    int count;
    int prescale = 1; // valid values: 1, 8, 64, 256, 1024
    do {
        count = (int) (targetSecs / (timerSteps * (float) prescale));
        if (count < 65535) // Timer 1 is 16-bits wide
            break;
        prescale *= 8;
    } while (prescale <= 1024);
    if (prescale > 1024) // time too long
        return false;
    if (prescale == 1 && count < 100) // time too short
        return false;

    OCR1A = count; // Eg, 200 = 0.1ms - I found 1ms gives rough acceleration
    // turn on CTC mode (Clear Timer on Compare Match):
    TCCR1B |= (1 << WGM12);
    // Set CS10 and CS12 bits for 1024 prescaler:
    // CS12 CS11 CS10
    // 0 0 0 no clock source, Timer/counter stopped
    // 0 0 1 clk/1 no prescaling
    // 0 1 0 clk/8
    // 0 1 1 clk/64
    // 1 0 0 clk/256
    // 1 0 1 clk/1024
    // 1 1 0 external clock on T1 pin, falling edge
    // 1 1 1 external clock on T1 pin, rising edge
    switch (prescale) {
        case 1:
            TCCR1B |= (1 << CS10); // 0 0 1
            break;
        case 8:
            TCCR1B |= (1 << CS11); // 0 1 0
            break;
        case 64:
            TCCR1B |= (1 << CS11) & (1 << CS10); // 0 1 1
            break;
        case 256:
            TCCR1B |= (1 << CS12); // 1 0 0
            break;
        case 1024:
            TCCR1B |= (1 << CS12) & (1 << CS10); // 1 0 1
            break;
        default:
            return false;
    }
    // enable timer compare interrupt:
    TIMSK1 |= (1 << OCIE1A);
    // enable global interrupts:
    sei();

    return true;
}
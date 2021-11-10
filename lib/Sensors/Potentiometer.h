#ifndef FLOWER_POTENTIOMETER_H
#define FLOWER_POTENTIOMETER_H

#include <Arduino.h>
#include <RunningMedian.h>

class Potentiometer {
public:
    Potentiometer(uint8_t potentiometer_pin, int low, int high);

    /**
     * Get the adjusted value from the potentiometer pin.
     * @returns The adjusted value between @see low and @high set in the constructor.
     */
    int value();

private:
    uint8_t potentiometer_pin;
    int low;
    int high;
    RunningMedian runningMedian = RunningMedian(19);
};


#endif //FLOWER_POTENTIOMETER_H

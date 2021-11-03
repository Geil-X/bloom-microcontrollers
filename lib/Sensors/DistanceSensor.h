#ifndef FLOWER_DISTANCESENSOR_H
#define FLOWER_DISTANCESENSOR_H

#include <Arduino.h>
#include <RunningMedian.h>


class DistanceSensor {
public:
    DistanceSensor(uint8_t trigger_pin, uint8_t echo_pin);

    /**
     * @return The distance to the closest object in centimeters.
     */
    float distanceCm(float max = 150);

    /**
     * @return The distance to the closest object in inches.
     */
    float distanceIn(float max = 13 * 12);

    /**
     * @return The distance to the closest object in feet.
     */
    float distanceFt(float max = 5);

private:
    /**
     * Get the duration to the closest object in microseconds. The duration is
     * halved so that the duration is only the distance to the object and not
     * the duration to and from.
     *
     * This duration is smoothed out by taking the median value of ever n'th
     * sample. This will affect your outputs negatively if functions are not
     * sampled fast enough.
     *
     * @param timeout_microseconds The maximum time that the sensor will wait
     *     for a response. After the timout period, the sensor will return
     *     half the timeout duration (the max time it could have measured).
     * @return The duration to the closest object in microseconds.
     */
    float durationMicroSeconds(unsigned long timeout_microseconds);

    RunningMedian runningDuration = RunningMedian(19);

    uint8_t trigger_pin;
    uint8_t echo_pin;
};


#endif //FLOWER_DISTANCESENSOR_H

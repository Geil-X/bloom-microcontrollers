#ifndef FLOWER_DISTANCESENSOR_H
#define FLOWER_DISTANCESENSOR_H

#include <Arduino.h>
#include <RunningMedian.h>


class DistanceSensor {
public:
    DistanceSensor(uint8_t triggerPin, uint8_t echoPin);

    /**
     * @return The distance to the closest object in centimeters. If no object
     *         is found then NAN is returned.
     */
    float distanceCm();

    /**
     * @return The distance to the closest object in inches. If no object
     *         is found then NAN is returned.
     */
    float distanceIn();

    /**
     * @return The distance to the closest object in inches. If no object
     *         is found then NAN is returned.
     */
    float distanceFt();

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
     * @return The duration to the closest object in microseconds.
     */
    float durationMicroSeconds();

    RunningMedian runningDuration = RunningMedian(19);

    uint8_t triggerPin;
    uint8_t echoPin;
};


#endif //FLOWER_DISTANCESENSOR_H

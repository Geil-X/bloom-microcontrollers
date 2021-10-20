#ifndef FLOWER_DISTANCESENSOR_H
#define FLOWER_DISTANCESENSOR_H

#include <Arduino.h>

// Constants for the speed of sound
#define MICROSECONDS_PER_INCH 74.0524781f
#define MICROSECONDS_PER_CM 29.154519f

class DistanceSensor {
public:
    DistanceSensor(uint8_t triggerPin, uint8_t echoPin);

    /**
     * @return The distance to the closest object in centimeters. If no object
     *         is found then NAN is returned.
     */
    float distanceCm() const;

    /**
     * @return The distance to the closest object in inches. If no object
     *         is found then NAN is returned.
     */
    float distanceIn() const;

private:
    /**
     * Get the duration to the closest object in microseconds. The duration is
     * halved so that the duration is only the distance to the object and not
     * the duration to and from.
     *
     * @return The duration to the closest object in microseconds.
     */
    float durationMicroSeconds() const;

    uint8_t triggerPin;
    uint8_t echoPin;
};


#endif //FLOWER_DISTANCESENSOR_H

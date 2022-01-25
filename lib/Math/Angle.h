#ifndef FLOWER_ANGLE_H
#define FLOWER_ANGLE_H

#include <Arduino.h>

/**
 * Representation of an angle represented discrete integer.
 *
 * Value [0, 360) degrees with a minimum step of 1.40625 degrees
 * Value [0, 2pi) radians with a minimum step of 0.02454 radians
 */
typedef uint8_t angle8;

/**
 * Representation of an angle represented discrete integer.
 *
 * Value [0, 360) degrees with a minimum step of 0.0054931608 degrees
 * Value [0, 2pi) radians with a minimum step of 0.0000958734 radians
 */
typedef uint16_t angle16;

/**
 * Get the shortest distance between two angles.
 * @param from The first angle
 * @param to The second angle
 * @return The shortest modular distance between two angles.
 */
inline angle8 minDelta8(angle8 from, angle8 to) {
    angle8 delta = max(from, to) - min(from, to);
    if (delta > 127) { // Half of UINT8_MAX
        return UINT8_MAX - delta;
    }
    return delta;
}

/**
 * Get the shortest distance between two angles.
 * @param from The first angle
 * @param to The second angle
 * @return The longest modular distance between two angles.
 */
inline angle8 maxDelta8(angle8 from, angle8 to) {
    angle8 delta = max(from, to) - min(from, to);
    if (delta < 128) { // Half of UINT8_MAX
        return UINT8_MAX - delta;
    }
    return delta;
}

/**
 * Get the shortest distance between two angles.
 * @param from The first angle
 * @param to The second angle
 * @return The shortest modular distance between two angles.
 */
inline angle16 minDelta16(angle16 from, angle16 to) {
    angle16 delta = max(from, to) - min(from, to);
    if (delta > 32767) { // Half of UINT16_MAX
        return UINT16_MAX - delta;
    }
    return delta;
}

/**
 * Get the shortest distance between two angles.
 * @param from The first angle
 * @param to The second angle
 * @return The longest modular distance between two angles.
 */
inline angle16 maxDelta16(angle16 from, angle16 to) {
    angle16 delta = max(from, to) - min(from, to);
    if (delta < 32768) { // Half of UINT16_MAX
        return UINT16_MAX - delta;
    }
    return delta;
}

#endif //FLOWER_ANGLE_H

#ifndef FLOWER_ANGLE_H
#define FLOWER_ANGLE_H

#include <Arduino.h>

struct Angle {
public:
    uint8_t theta;

    // default values are UNINITIALIZED
    inline Angle() __attribute__((always_inline)) = default;

    inline Angle(uint8_t theta)  __attribute__((always_inline)): theta(theta) {}

    /// allow copy construction
    inline Angle(const Angle &angle) __attribute__((always_inline)) = default;

    /// Get the shortest angle between two different angles
    inline Angle shortestDistance(Angle& rhs) __attribute((always_inline)){
        uint8_t delta = max(this->theta, rhs.theta) - min(this->theta, rhs.theta);
        return min(delta, 255 - delta);
    }

    /// Get the longest angle between two different angles
    inline Angle longestDistance(Angle& rhs) __attribute((always_inline)){
        uint8_t delta = max(this->theta, rhs.theta) - min(this->theta, rhs.theta);
        return max(delta, 255 - delta);
    }

    inline Angle &operator=(const Angle &rhs) __attribute__((always_inline)) = default;

    /// allow assignment from uint8_t
    inline Angle &operator=(uint8_t rhs) __attribute__((always_inline)) {
        this->theta = rhs;
        return *this;
    };

    inline bool operator==(Angle rhs) const __attribute__((always_inline)) {
        return this->theta == rhs.theta;
    }

    /// Rotate this rhs clockwise by another rhs
    inline Angle &operator+=(Angle rhs) __attribute((always_inline)) {
        this->theta += rhs.theta;
        return *this;
    };

    /// Rotate this rhs counter clockwise by another rhs
    inline Angle &operator-=(Angle rhs) __attribute((always_inline)) {
        this->theta -= rhs.theta;
        return *this;
    };

    /// Get the first rhs rotated clockwise by the second rhs
    inline Angle operator+(Angle rhs) const __attribute((always_inline)) {
        return this->theta + rhs.theta;
    };

    /// Get the first rhs rotated counter clockwise by the second rhs
    inline Angle operator-(Angle rhs) const __attribute((always_inline)) {
        return this->theta - rhs.theta;
    };

    /// Compare two angles
    inline bool operator<(Angle rhs) const __attribute((always_inline)) {
        return this->theta < rhs.theta;
    };

    /// Compare two angles
    inline bool operator<=(Angle rhs) const __attribute((always_inline)) {
        return this->theta <= rhs.theta;
    };

    /// Compare two angles
    inline bool operator>(Angle rhs) const __attribute((always_inline)) {
        return this->theta > rhs.theta;
    };

    /// Compare two angles
    inline bool operator>=(Angle rhs) const __attribute((always_inline)) {
        return this->theta >= rhs.theta;
    };
};

#endif //FLOWER_ANGLE_H

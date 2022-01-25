#ifndef FLOWER_POINT2D_H
#define FLOWER_POINT2D_H

#include <Arduino.h>

class Point2D {
public:
    Point2D(float x, float y);

    /**
     * Create a point object from the polar form.
     * @param r The radius of the point
     * @param theta The angle of the point in radians
     * @return A new point object from the polar coordinates
     */
    static Point2D fromPolar(float r, float theta);

    String toString() const;

    float x;
    float y;
};


#endif //FLOWER_POINT2D_H

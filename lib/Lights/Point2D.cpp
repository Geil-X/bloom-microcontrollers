#include "Point2D.h"

#include "math.h"

Point2D::Point2D(float x, float y) : x(x), y(y) {}

Point2D Point2D::fromPolar(float r, float theta) {
    return {r * (float) cos((double) theta),
            r * (float) sin((double) theta)};
}

String Point2D::toString() const {
    return {"(" + String(x) + ", " + String(y) + ")"};
}

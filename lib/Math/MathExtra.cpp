#include <Arduino.h>
#include <math.h>
#include <MathExtra.h>

float map(float amt, float from_low, float from_high, float to_low, float to_high) {
    return to_low + (amt - from_low) * (to_high - to_low) / (from_high - from_low);
}

float identity(float x) { return x; }

float inverse(float x) { return -x; }

float ncos(float x, float period) {
    return (float) ((1. - cos(TWO_PI * x / period)) / 2.);
}

float nsin(float x, float period) {
    return (float) ((1. - sin(TWO_PI * x / period)) / 2.);
}

float linearRamp(float x, float start, float stop) {
    return constrain((x - start) / (stop - start), 0, 1);
}
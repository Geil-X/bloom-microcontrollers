#include <Arduino.h>
#include <math.h>
#include "MathExtra.h"

float zero(float _) { return 0; }

float identity(float x) { return x; }

float lerp(float t, float a, float b) {
    t = constrain(t, 0, 1);
    return (1 - t) * a + t * b;
}

float flerp(float t, float a, float b) {
    t = constrain(t, 0, 1);
    return (1 - t) * a + t * b;
}

float linearRamp(float x, float start, float stop) {
    return constrain((x - start) / (stop - start), 0, 1);
}

float inverse(float x) { return 1 - x; }

// Trigonometry

float cos(float t, float cycles) {
    return (float) cos((double) t * cycles);
}

float ncos(float t, float cycles) {
    return (float) ((1 + cos(TWO_PI * t * cycles)) / 2.);
}

float incos(float t, float cycles) {
    return (float) ((1. - cos(TWO_PI * t * cycles)) / 2.);
}

float isin(float t, float cycles) {
    return (float) -sin((double) t * cycles);
}

float sin(float t, float cycles) {
    return (float) sin((double) t * cycles);
}

float insin(float x, float cycles) {
    return (float) ((1. - sin(TWO_PI * x * cycles)) / 2.);
}

/// Get a random floating point number from 0-1
float randomFloat() {

    return static_cast <float> (random()) / static_cast <float> (RANDOM_MAX);
}

/// Get a random floating point number from 0-max
float randomFloat(float max) {
    return static_cast <float> (random()) / (static_cast <float> ((float) RANDOM_MAX / max));
}

/// Get a random floating point number from min-max
float randomFloat(float min, float max) {
    return min + (float) random() / ((float) RANDOM_MAX / (max - min));
}
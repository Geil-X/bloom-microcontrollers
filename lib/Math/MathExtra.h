#ifndef FLOWER_MATHEXTRA_H
#define FLOWER_MATHEXTRA_H

#include <FastLED.h>

//#define map(amt, from_low, from_high, to_low, to_high) to_low + (amt - from_low) * (to_high - to_low) / (from_high - from_low)


//float map(float amt, float from_low, float from_high, float to_low, float to_high);


/**
 * map16: map from one full-range 16-bit value into a narrower range of 16-bit values, possibly a range of hues.
 * E.g. map myValue into a hue in the range blue..purple..pink..red hue = map16( myValue, HUE_BLUE, HUE_RED);
 * Combines nicely with the waveform functions (like sin16, etc) to produce continuous hue gradients back and forth:
 *      hue = map16( sin16( myValue), HUE_BLUE, HUE_RED);
 * Mathematically simiar to lerp16by16, but arguments are more like Arduino's "map"; this function is similar to
 *      map( in, 0, 255, rangeStart, rangeEnd)
 * but faster and specifically designed for 16-bit values.
 * @param in
 * @param rangeStart
 * @param rangeEnd
 * @return
 */
static inline uint16_t map16(uint16_t in, uint16_t rangeStart, uint16_t rangeEnd) {
    uint16_t rangeWidth = rangeEnd - rangeStart;
    uint16_t out = scale16(in, rangeWidth);
    out += rangeStart;
    return out;
}

float zero(float _);

float identity(float x);

/**
 * Linearly interpolate between two values.
 * @param t The interpolation value. Value is clamped in the range [0, 1]. 0 just the a value, and 1 is just the b value.
 * @param a The initial value.
 * @param b The final value.
 * @return The interpolated value between a and b.
 */
float lerp(float t, float a, float b);

/**
 * Linearly interpolate between two values.
 * @param a The initial value.
 * @param b The final value.
 * @param t The interpolation value. Value is clamped in the range [0, 1]. 0 just the a value, and 1 is just the b value.
 * @return The interpolated value between a and b.
 */
float flerp(float a, float b, float t);

/** Invert a value within the range 0 -> 1 */
float inverse(float x);

/**
 * Linear ramping function. If the input value is less than the start value
 * return 0. If the input value is greater than the stop value return 1.
 * If the input value is between the start and stop value, the value is the
 * linearly interpolated value between 0 and 1 for that interval.
 *
 * @note
 *     0 ___/‾‾‾‾‾‾ 1
 *
 * @param x  Input value.
 * @param start The value to start ramping up.
 * @param stop The value to complete ramping.
 * @return Ramped value from 0 to 1.
 */
float linearRamp(float x, float start, float stop);

// Trigonometry

float cos(float t, float cycles);

/**
 * Normalized cosine function with adjustable period. The function is
 * normalized by remapping the typical output from -1 to 1 into the range of
 * 0 to 1. The function also has an adjustable period with which the cosine
 * function oscillates.
 *
 * @param t Input value
 * @param cycles The number of cycles to perform in the time window between 0 and 1
 * @return Normalized cosine function with results between 0 and 1 inclusive.
 */
float ncos(float t, float cycles = 1.);

/**
 * Inverted and normalized cosine function with adjustable period. The function is
 * normalized by remapping the typical output from -1 to 1 into the range of
 * 0 to 1. The function also has an adjustable period with which the cosine
 * function oscillates.
 *
 * @param t Input value
 * @param cycles The number of cycles to perform in the time window between 0 and 1
 * @return Normalized cosine function with results between 0 and 1 inclusive.
 */
float incos(float t, float cycles = 1.);

float isin(float t, float cycles = 1);
float sin(float t, float cycles = 1);

/**
 * Inverted and normalized sine function with adjustable period. The function is
 * normalized by remapping the typical output from -1 to 1 into the range of
 * 0 to 1. The function also has an adjustable period with which the sine
 * function oscillates.
 *
 * @param x Input value
 * @param period The period the adjust the output with.
 * @return Normalized cosine function with results between 0 and 1 inclusive.
 */
float insin(float x, float cycles = 1.);

/**
 * Generate a random floating point number between min and max
 * @param min The minimum floating point value you are looking for
 * @param max The maximum floating point value you are looking for
 * @return A random floating point number between min and max
 */
float randomFloat(float min, float max);

#endif //FLOWER_MATHEXTRA_H

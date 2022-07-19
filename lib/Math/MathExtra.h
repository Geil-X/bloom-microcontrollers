#ifndef FLOWER_MATHEXTRA_H
#define FLOWER_MATHEXTRA_H

#include <FastLED.h>

#include <Types.h>

/**
 * map16: map from one full-range 16-bit value into a narrower range of 16-bit values, possibly a range of hues.
 * E.g. map myValue into a hue in the range blue..purple..pink..red hue = map16( myValue, HUE_BLUE, HUE_RED);
 * Combines nicely with the waveform functions (like sin16, etc) to produce continuous hue gradients back and forth:
 *      hue = map16( sin16( myValue), HUE_BLUE, HUE_RED);
 * Mathematically similar to lerp16by16, but arguments are more like Arduino's "map"; this function is similar to
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


static inline uint16_t map16Max(uint16_t x, uint16_t in_max, uint16_t out_max) {
    return x * (out_max / in_max);
}

static inline float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/**
 * Keep the 8 most significant bits of a 16 bit fractional number and convert it into the smaller 8 bit fractional
 * @param f The input 16 bit fractional number
 * @return The 8 bit fractional number
 */
static inline fract8 fract16ToFract8(fract16 f) {
    return (fract8) (f >> 8);
}

static inline Percentage randomPercentage() {
    return random16();
}


#endif //FLOWER_MATHEXTRA_H

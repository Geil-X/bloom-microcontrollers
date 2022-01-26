#ifndef FLOWER_MATHEXTRA_H
#define FLOWER_MATHEXTRA_H

#include <FastLED.h>

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

#endif //FLOWER_MATHEXTRA_H

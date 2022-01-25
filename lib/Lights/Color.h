#ifndef FLOWER_COLOR_H
#define FLOWER_COLOR_H

#include <Arduino.h>
#include "FastLED.h"

/**
 * Value or brightness of a color in HSV space. This gives
 * more human readable values to the 0 -> 255 value space;
 */
enum Value {
    VALUE_BARELY_ON = 20,
    VALUE_DIM = 50,
    VALUE_VIBRANT = 255,
};

class Color {
public:
    static CRGB Red;
    static CRGB Orange;
    static CRGB Yellow;
    static CRGB LimeYellow;
    static CRGB LimeGreen;
    static CRGB Green;
    static CRGB MossGreen;
    static CRGB Cyan;
    static CRGB Teal;
    static CRGB Blue;
    static CRGB BluePurple;
    static CRGB Purple;
    static CRGB PurpleMagenta;
    static CRGB Magenta;
    static CRGB RedMagenta;
};


class FLAB {
public:
    FLAB(float l, float a, float b);

    static FLAB fromRgb(CRGB rgb);
    CRGB toRgb() const;
    String toString() const;

    float l;
    float a;
    float b;
};

class FXYZ {
public:
    FXYZ(float x, float y, float z);

    static FXYZ fromRgb(CRGB rgb);
    CRGB toRgb() const;
    String toString() const;

    float x;
    float y;
    float z;
};


class FRGB {
public:
    FRGB(float r, float g, float b);

    float r;
    float g;
    float b;
};


#endif //FLOWER_COLOR_H

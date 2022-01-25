#ifndef FLOWER_PROCEDURALCOLOR_H
#define FLOWER_PROCEDURALCOLOR_H

#include "FastLED.h"
#include "Color.h"

// Procedural color palettes
// https://iquilezles.org/www/articles/palettes/palettes.htm


class ProceduralColor {
public:
    ProceduralColor(FRGB a, FRGB b, FRGB c, FRGB d);
    CRGB color(float seconds) const;

private:
    static float cosEq(float t, float w, float x, float y, float z);

    FRGB a;
    FRGB b;
    FRGB c;
    FRGB d;

};

class Gradient1D {
public:
    Gradient1D(const CRGB &from, const CRGB &to);
    CRGB colorAt(float x) const;

private:
    CRGB from;
    CRGB to;
};

class Gradient2D {
public:
    Gradient2D(CRGB topLeft, CRGB topRight, CRGB bottomRight, CRGB bottomLeft);

    CRGB colorAt(float x, float y);

private:
    Gradient1D top;
    Gradient1D bottom;
};


#endif //FLOWER_PROCEDURALCOLOR_H

// A lot of code was adapted from
// https://getreuer.info/posts/colorspace/index.html

#include "ProceduralColor.h"
#include <Arduino.h>
#include "FastLED.h"
#include <math.h>
#include <MathExtra.h>
#include "Color.h"

ProceduralColor::ProceduralColor(FRGB a, FRGB b, FRGB c, FRGB d) : a(a), b(b), c(c), d(d) {}


CRGB ProceduralColor::color(float seconds) const {
    return {(uint8_t) (255 * cosEq(seconds, a.r, b.r, c.r, d.r)),
            (uint8_t) (255 * cosEq(seconds, a.g, b.g, c.g, d.g)),
            (uint8_t) (255 * cosEq(seconds, a.b, b.b, c.b, d.b))
    };
}

float ProceduralColor::cosEq(float t, float w, float x, float y, float z) {
    return w + x * (float) cos(2 * PI * (y * t + z));
}

Gradient1D::Gradient1D(const CRGB &from, const CRGB &to) :
        from(from),
        to(to) {}

CRGB Gradient1D::colorAt(float x) const {
    x = constrain(x, 0, 1);
    return {(uint8_t) lerp(x, from.r, to.r),
            (uint8_t) lerp(x, from.g, to.g),
            (uint8_t) lerp(x, from.b, to.b)};
}

Gradient2D::Gradient2D(CRGB topLeft, CRGB topRight, CRGB bottomRight, CRGB bottomLeft)
        : top(topLeft, topRight),
          bottom(bottomLeft,
                 bottomRight) {}

CRGB Gradient2D::colorAt(float x, float y) {
    Gradient1D topToBottom = Gradient1D(top.colorAt(x), bottom.colorAt(x));
    return topToBottom.colorAt(y);
}

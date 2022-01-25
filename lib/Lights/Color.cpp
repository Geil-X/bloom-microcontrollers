//
// Created by Tommy Waters on 1/12/22.
//

#include "Color.h"

#include <Arduino.h>
#include "FastLED.h"
#include <math.h>
#include <MathExtra.h>

// Colors

CRGB Color::Red = CHSV(0, 255, 255);
CRGB Color::Orange = CHSV(15, 255, 255);
CRGB Color::Yellow = CHSV(26, 255, 255);
CRGB Color::LimeYellow = CHSV(37, 255, 255);
CRGB Color::LimeGreen = CHSV(66, 255, 255);
CRGB Color::Green = CHSV(66, 255, 255);
CRGB Color::MossGreen = CHSV(90, 255, 255);
CRGB Color::Teal = CHSV(100, 255, 255);
CRGB Color::Cyan = CHSV(120, 255, 255);
CRGB Color::Blue = CHSV(160, 255, 255);
CRGB Color::BluePurple = CHSV(186, 255, 255);
CRGB Color::Purple = CHSV(202, 255, 255);
CRGB Color::PurpleMagenta = CHSV(215, 255, 255);
CRGB Color::Magenta = CHSV(240, 255, 255);
CRGB Color::RedMagenta = CHSV(248, 255, 255);


// Helper macros

/** @brief XYZ color of the D65 white point */
#define WHITEPOINT_X    0.950456f
#define WHITEPOINT_Y    1.0f
#define WHITEPOINT_Z    1.088754f

/** @brief Min of A and B */
#define MIN(A, B)    (((A) <= (B)) ? (A) : (B))

/** @brief Max of A and B */
#define MAX(A, B)    (((A) >= (B)) ? (A) : (B))

/** @brief Min of A, B, and C */
#define MIN3(A, B, C)    (((A) <= (B)) ? MIN(A,C) : MIN(B,C))

/** @brief Max of A, B, and C */
#define MAX3(A, B, C)    (((A) >= (B)) ? MAX(A,C) : MAX(B,C))

/**
 * @brief sRGB gamma correction, transforms R to R'
 * http://en.wikipedia.org/wiki/SRGB
 */
#define GAMMACORRECTION(t)    \
    (((t) <= 0.0031306684425005883) ? \
    (12.92*(t)) : (1.055*pow((t), 0.416666666666666667) - 0.055))

/**
 * @brief Inverse sRGB gamma correction, transforms R' to R
 */
#define INVGAMMACORRECTION(t)    \
    (((t) <= 0.0404482362771076) ? \
    ((t)/12.92) : pow(((t) + 0.055)/1.055, 2.4))

/**
 * @brief CIE L*a*b* f function (used to convert XYZ to L*a*b*)
 * http://en.wikipedia.org/wiki/Lab_color_space
 */
#define LABF(t)    \
    ((t >= 8.85645167903563082e-3f) ? \
    pow(t,0.333333333333333) : (841.0f/108.0f)*(t) + (4.0f/29.0f))

/**
 * @brief CIE L*a*b* inverse f function
 * http://en.wikipedia.org/wiki/Lab_color_space
 */
#define LABINVF(t)    \
    ((t >= 0.206896551724137931f) ? \
    ((t)*(t)*(t)) : (108.0f/841.0f)*((t) - (4.0f/29.0f)))

// Conversion functions

/**
 * @brief Transform sRGB to CIE XYZ with the D65 white point
 */
FXYZ rgb2Xyz(const CRGB &rgb) {
    float R = INVGAMMACORRECTION(rgb.r);
    float G = INVGAMMACORRECTION(rgb.g);
    float B = INVGAMMACORRECTION(rgb.b);
    return {(float) (0.4123955889674142161 * R + 0.3575834307637148171 * G + 0.1804926473817015735 * B),
            (float) (0.2125862307855955516 * R + 0.7151703037034108499 * G + 0.07220049864333622685 * B),
            (float) (0.01929721549174694484 * R + 0.1191838645808485318 * G + 0.9504971251315797660 * B)};
}

CRGB xyz2Rgb(const FXYZ &xyz) {
    auto r = (float) (3.2406 * xyz.x - 1.5372 * xyz.y - 0.4986 * xyz.z);
    auto g = (float) (-0.9689 * xyz.x + 1.8758 * xyz.y + 0.0415 * xyz.z);
    auto b = (float) (0.0557 * xyz.x - 0.2040 * xyz.y + 1.0570 * xyz.z);

    float min = MIN3(r, g, b);

    // Force nonnegative values so that gamma correction is well-defined.
    if (min < 0) {
        r -= min;
        g -= min;
        b -= min;
    }

    // Transform from RGB to R'G'B'
    return {(uint8_t) GAMMACORRECTION(r),
            (uint8_t) GAMMACORRECTION(g),
            (uint8_t) GAMMACORRECTION(b)};
}

FLAB xyz2Lab(const FXYZ &xyz) {
    float x = LABF(xyz.x / WHITEPOINT_X);
    float y = LABF(xyz.y / WHITEPOINT_Y);
    float z = LABF(xyz.z / WHITEPOINT_Z);
    return {116 * y - 16,
            500 * (x - y),
            200 * (y - z)};
}

FXYZ lab2Xyz(const FLAB lab) {
    float L = (lab.l + 16) / 116;
    float a = L + lab.a / 500;
    float b = L - lab.b / 200;
    return {(WHITEPOINT_X * LABINVF(a)),
            (WHITEPOINT_Y * LABINVF(L)),
            (WHITEPOINT_Z * LABINVF(b))};
}

// Class Methods

FLAB::FLAB(float l, float a, float b) : l(l), a(a), b(b) {}

FLAB FLAB::fromRgb(CRGB rgb) {
    return xyz2Lab(rgb2Xyz(rgb));
}

CRGB FLAB::toRgb() const {
    return xyz2Rgb(lab2Xyz(*this));
}

String FLAB::toString() const {
    return "Lab(" + String(l) + ", " + String(a) + ", " + String(b) + ")";
}

FXYZ::FXYZ(float x, float y, float z) : x(x), y(y), z(z) {}

FXYZ FXYZ::fromRgb(CRGB rgb) {
    return rgb2Xyz(rgb);
}

CRGB FXYZ::toRgb() const {
    return xyz2Rgb(*this);
}

String FXYZ::toString() const {
    return "Xyz(" + String(x) + ", " + String(y) + ", " + String(z) + ")";
}

FRGB::FRGB(float r, float g, float b) : r(r), g(g), b(b) {}
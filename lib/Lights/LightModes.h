#ifndef FLOWER_LIGHTMODES_H
#define FLOWER_LIGHTMODES_H

#include <FastLED.h>
#include "Color.h"

//struct NoisyGradient {
//    CRGB backgroundColor;     // Base color that is the primary color
//    CRGB accentColor;         // The color that will be popping as the secondary color
//    float flickerRate;        // "Roughly" Flickers per second (per led)
//    float ledDistance;        // Color distance between led's (good range 0.5 to 5)
//    float flickerSharpness;   // Normal range (0.25 -> 10 and up to 100) lower is a slower flicker and higher is a more crisp flicker
//    float rotationsPerSecond; // The number of rotations clockwise per second that the animation will take
//    bool jumpy;               // If true the base color becomes more previlent and accent color jumps in faster
//};
//
//void noisyGradient(NoisyGradient preferences, float t, CRGB leds[], uint8_t num_leds);
//
///**
// * Fire effect with a red background and a yellow crackling.
// */
//void fireCrackle(float t, CRGB leds[], uint8_t num_leds);
//
///**
// * Color effect where there is a dim purple background and little flickering lightening
// * bug "chirps" of a lime green yellow
// */
//void fireflies(float t, CRGB leds[], uint8_t num_leds);
//
//void lightning(float t, CRGB leds[], uint8_t num_leds);
//
//void slowGradient(CRGB background, CRGB accent, float t, CRGB leds[], uint8_t num_leds);
//
#endif //FLOWER_LIGHTMODES_H

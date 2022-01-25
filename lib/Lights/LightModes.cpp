#include "LightModes.h"

#include <Arduino.h>

#include "Color.h"
#include "MathExtra.h"
#include "ProceduralColor.h"
#include "noise.h"
#include "Point2D.h"

//void noisyGradient(NoisyGradient preferences, float t, CRGB leds[], uint8_t num_leds) {
//    Gradient1D colorGradient = Gradient1D(preferences.backgroundColor, preferences.accentColor);
//
//    // To make the ring LED continuous in the noise field, points are
//    // converted into a ring around the center space in the unit circle (-1 <= x & y <= 1)
//    for (int i = 0; i < num_leds; i++) {
//        float angle = map((float) i, 0, num_leds, 0, 2 * PI);
//        angle += preferences.rotationsPerSecond * t / (2.0f * (float) PI);
//        Point2D ledLocation = Point2D::fromPolar(preferences.ledDistance, angle);
//
//        float colorGradientNoise = noise3d(ledLocation.x, ledLocation.y, t * preferences.flickerRate);
//
//        if (preferences.jumpy) {
//            colorGradientNoise = 1 - abs(map(colorGradientNoise, 0, 1, -1, 1));
//        }
//
//        colorGradientNoise = (float) pow((double) colorGradientNoise, (double) preferences.flickerSharpness);
//
//        leds[i] = colorGradient.colorAt(colorGradientNoise);
//    }
//}
//
////void fireCrackle(float t, CRGB leds[], uint8_t num_leds) {
////    NoisyGradient fireCrackleGradient = {
////            .backgroundColor = Color::Red,
////            .accentColor = Color::Yellow,
////            .flickerRate = 2.5,
////            .ledDistance = 2.5,
////            .flickerSharpness = 10,
////            .rotationsPerSecond = 0.f,
////            .jumpy = true,
////    };
////    return noisyGradient(fireCrackleGradient, t, leds, num_leds);
////}
//
////void noisyGradient(NoisyGradient preferences, float t, CRGB leds[], uint8_t num_leds) {
//void fireCrackle(float t, CRGB leds[], uint8_t num_leds) {
//    float flickerRate = 3.f;
//    float ledDistance = 2.5f;
//    float flickerSharpness = 1.5f;
//
//    // To make the ring LED continuous in the noise field, points are
//    // converted into a ring around the center space in the unit circle (-1 <= x & y <= 1)
//    for (int i = 0; i < num_leds; i++) {
//        float angle = map((float) i, 0, num_leds, 0, 2 * PI);
//        Point2D ledLocation = Point2D::fromPolar(ledDistance, angle);
//
//        float colorGradientNoise = noise3d(ledLocation.x, ledLocation.y, t * flickerRate);
//        // Jumpy colors
//        colorGradientNoise = abs(map(colorGradientNoise, 0, 1, -1, 1));
//
//        colorGradientNoise = (float) pow((double) colorGradientNoise, (double) flickerSharpness);
//
//        auto temperature = (uint8_t) (255 * colorGradientNoise);
//        leds[i] = HeatColor(temperature);
//    }
//}
//
//void fireflies(float t, CRGB leds[], uint8_t num_leds) {
//    NoisyGradient fireflyColorGradient = {
//            .backgroundColor  = Color::BluePurple.fadeLightBy(60),
//            .accentColor = Color::LimeYellow,
//            .flickerRate = 0.05,
//            .ledDistance = 2,
//            .flickerSharpness = 15,
//            .rotationsPerSecond = 1.f,
//            .jumpy = true,
//    };
//    return noisyGradient(fireflyColorGradient, t, leds, num_leds);
//}
//
//void lightning(float t, CRGB leds[], uint8_t num_leds) {
//    NoisyGradient slowColorGradient = {
//            .backgroundColor  = CHSV(180, 50, 30),
//            .accentColor  = CHSV(40, 170, 200),
//            .flickerRate = 0.25,
//            .ledDistance = 5,
//            .flickerSharpness = 200,
//            .rotationsPerSecond = 0.f,
//            .jumpy = true,
//    };
//    return noisyGradient(slowColorGradient, t, leds, num_leds);
//
//}
//
//void slowGradient(CRGB background, CRGB accent, float t, CRGB leds[], uint8_t num_leds) {
//    NoisyGradient slowColorGradient = {
//            .backgroundColor  = background,
//            .accentColor = accent,
//            .flickerRate = 0.75,
//            .ledDistance = 3,
//            .flickerSharpness = 1.5,
//            .rotationsPerSecond = 0.f,
//            .jumpy = false,
//    };
//    return noisyGradient(slowColorGradient, t, leds, num_leds);
//}
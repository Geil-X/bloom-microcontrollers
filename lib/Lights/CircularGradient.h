#ifndef FLOWER_CIRCULARGRADIENT_H
#define FLOWER_CIRCULARGRADIENT_H

#include <Arduino.h>
#include <FastLED.h>
#include <Angle.h>
#include "MathExtra.h"


struct CircularColorNode {
    CircularColorNode() : color(CRGB(0, 0, 0)), angle(0) {}

    CircularColorNode(CRGB color, uint8_t position) : color(color), angle(position) {}

    CircularColorNode(CRGB color, Angle position) : color(color), angle(position) {}

    CRGB color;
    Angle angle;
};

bool sortCircularNodeByAngle(const CircularColorNode *a, const CircularColorNode *b) {
    return a->angle < b->angle;
}

/**
 * Get the color at a particular position in a circular gradient.
 * @param angle The location in the gradient to pick the color of.
 * @param gradient The array that describes the color gradient.
 * @param numColors The number of colors in the array gradient.
 * @return The color at a particular location in the circular gradient.
 *
 * @note gradient should be an array sorted by the angle of the color node
 */
CRGB colorAt(Angle angle, CircularColorNode *gradient[], uint8_t numColors) {
    // Invalid user input
    if (numColors == 0) return {0, 0, 0};

    // Check if the gradient even needs to be calculated
    for (uint8_t i = 0; i < numColors; i++) {
        if (angle == gradient[i]->angle) return gradient[i]->color;
    }

    uint8_t lastIndex = numColors - 1;

    // Color is at the circular boundary between the first color and 0
    if (angle < gradient[0]->angle) {
        Angle colorDistance = gradient[0]->angle.shortestDistance(gradient[lastIndex]->angle);
        uint8_t blendAmount = map(gradient[0]->angle.theta, 0, colorDistance.theta, 0, UINT8_MAX);
        return blend(gradient[0]->color, gradient[lastIndex]->color, blendAmount);
    }

    // Color is at the circular boundary between the last color and 255 (UINT8_MAX)
    if (angle > gradient[lastIndex]->angle) {
        Angle colorDistance = gradient[0]->angle + (Angle(255) - gradient[lastIndex]->angle);
        Angle inputDistance = gradient[lastIndex]->angle.shortestDistance(angle);
        uint8_t blendAmount = map(inputDistance.theta, 0, colorDistance.theta, 0, UINT8_MAX);
        return blend(gradient[lastIndex]->color, gradient[0]->color, blendAmount);
    }

    // Color is in the middle of the gradient. This is the generic logic for color interpolation.
    for (uint8_t i = 0; i < numColors - 1; i++) {
        if (angle > gradient[i]->angle && angle < gradient[i + 1]->angle) {
            Angle colorDistance = gradient[i]->angle.shortestDistance(gradient[i + 1]->angle);
            Angle inputDistance = gradient[i]->angle.shortestDistance(angle);
            uint8_t blendAmount = map(inputDistance.theta, 0, colorDistance.theta, 0, UINT8_MAX);
            return blend(gradient[i]->color, gradient[i + 1]->color, blendAmount);
        }
    }

    // Some error occurred
    return {0, 0, 0};
}

#endif //FLOWER_CIRCULARGRADIENT_H
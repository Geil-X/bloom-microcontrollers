/**
 * This is the flowers emotional core. These basal emotions are changed by
 * external stimuli through it's input sensors. Changes in these emotions
 * change the external behavior of the flower.
 */

#ifndef FLOWER_PERSONALITY_H
#define FLOWER_PERSONALITY_H

#include <Arduino.h>
#include <FastLED.h>
#include "Flower.h"
#include "MathExtra.h"
#include "Angle.h"
#include "CircularGradient.h"

enum Speed {
    SLOW_SPEED = 500,
    MEDIUM_SPEED = 1000,
    FAST_SPEED = 3000,
};

enum Acceleration {
    SLOW_ACCELERATION = 500,
    MEDIUM_ACCELERATION = 1000,
    FAST_ACCELERATION = 3000,
};


class IEmotionState {
public:
    virtual void initFlower(Flower &flower) = 0;
    virtual void controlMotor(uint16_t millis, Flower &flower) = 0;
    virtual void controlLights(uint16_t millis, Flower &flower, CRGB leds[], uint8_t numLeds) = 0;
};

class Excited : IEmotionState {
#define NUM_GRADIENT_COLORS 3
public:
    void initFlower(Flower &flower) override {
        flower.setAcceleration(FAST_ACCELERATION);
        flower.setMaxSpeed(FAST_SPEED);
    }

    void controlMotor(uint16_t millis, Flower &flower) override {
        if (flower.isAtTarget()) {
            flower.openToAsync(random16());
        }
    }

    void controlLights(uint16_t millis, Flower &flower, CRGB leds[], uint8_t numLeds) override {
        angle8 globalOffset = inoise8(flower.getPosition() / 250) * 5;

        for (int i = 0; i < numLeds; i++) {
            angle8 ledAngle = qmul8(UINT8_MAX / numLeds, i);
            leds[i] = colorAt(ledAngle + globalOffset, circularGradient, NUM_GRADIENT_COLORS);
        }
    }

private:
    CircularColorNode *circularGradient[NUM_GRADIENT_COLORS] = {
            new CircularColorNode(CRGB::DarkBlue, 0),
            new CircularColorNode(CRGB::Olive, 255 / 3),
            new CircularColorNode(CRGB::Maroon, 255 / 3 * 2),
    };
};

/**
 * Motor Movement:
 *     Slowly go between closed and slightly closed.
 *
 * Coloring:
 *     Blue and purple lighting that slowly pulse in and out and rotate
 *     around the center.
 */
class Sleepy : IEmotionState {
#define NUM_GRADIENT_COLORS 3
public:
    void initFlower(Flower &flower) override {
        flower.setAcceleration(SLOW_ACCELERATION);
        flower.setMaxSpeed(SLOW_SPEED);
    }

    void controlMotor(uint16_t millis, Flower &flower) override {
        if (flower.isClosed()) {
            flower.openToAsync(UINT16_MAX / 4);
        }
        if (flower.isAtTarget()) {
            flower.closeAsync();
        }
    }

    void controlLights(uint16_t millis, Flower &flower, CRGB leds[], uint8_t numLeds) override {
        fract8 lerp = constrain((flower.getPosition() / UINT8_MAX), 0, UINT8_MAX / 4) * 4;
        for (int i = 0; i < numLeds; i++) {
            leds[i] = blend(closedColor, openColor, lerp);
        }
    };

private:
    CRGB openColor = CRGB::Purple;
    CRGB closedColor = CRGB::Blue;
};

/**
 * Motor Movement:
 *     Alternate between closed and almost closed.
 *
 * Coloring:
 *     Show flashes of color at jarring intervals.
 */
class Angry : IEmotionState {
public:
    void initFlower(Flower &flower) override {
        flower.setAcceleration(FAST_ACCELERATION);
        flower.setMaxSpeed(MEDIUM_SPEED);
    }

    void controlMotor(uint16_t millis, Flower &flower) override {
        if (flower.isClosed()) {
            flower.openToAsync(random16() / 2);
        }
        if (flower.isAtTarget()) {
            flower.closeAsync();
        }
    }

    void controlLights(uint16_t millis, Flower &flower, CRGB leds[], uint8_t numLeds) override {
        for (int i = 0; i < numLeds; i++) {
            leds[i] = CRGB::Red;
        }
    }
};


/**
 * Motor Movement:
 *     Wander open and closed "exploring" at a moderate pace.
 *
 * Coloring:
 *     Highly colorful and also wandering around as the motor moves.
 */
class Curious : IEmotionState {
#define NUM_GRADIENT_COLORS 3
public:
    void initFlower(Flower &flower) override {
        flower.setAcceleration(MEDIUM_ACCELERATION);
        flower.setMaxSpeed(MEDIUM_SPEED);
    }

    void controlMotor(uint16_t millis, Flower &flower) override {
        if (flower.isAtTarget()) {
            flower.openToAsync(random16());
        }
    }

    void controlLights(uint16_t millis, Flower &flower, CRGB leds[], uint8_t numLeds) override {
        angle8 globalOffset = inoise8(millis / 5) * 5;
        angle8 colorLerps[NUM_GRADIENT_COLORS] = {
                inoise8(millis / 2, 500),
                inoise8(millis / 2, 1000),
                inoise8(millis / 2, 1500)};

        for (int i = 0; i < NUM_GRADIENT_COLORS; i++) {
            circularGradient[i]->color = blend(gradient1[i], gradient2[i], colorLerps[i]);
        }

        for (int i = 0; i < numLeds; i++) {
            angle8 ledAngle = qmul8(UINT8_MAX / numLeds, i);
            leds[i] = colorAt(ledAngle + globalOffset, circularGradient, NUM_GRADIENT_COLORS);
        }
    }

private:
    CRGB gradient1[NUM_GRADIENT_COLORS] = {CRGB::Blue, CRGB::Coral, CRGB::Yellow};
    CRGB gradient2[NUM_GRADIENT_COLORS] = {CRGB::Olive, CRGB::Magenta, CRGB::Green};


    CircularColorNode *circularGradient[NUM_GRADIENT_COLORS] = {
            new CircularColorNode(CRGB::Maroon, 0),
            new CircularColorNode(CRGB::Purple, 255 / 3),
            new CircularColorNode(CRGB::Magenta, 255 / 3 * 2),
    };
};

#endif //FLOWER_PERSONALITY_H

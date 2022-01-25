#include <Arduino.h>
#include <AccelStepper.h>
#include <TMCStepper.h>

// 24 LED Neopixel ring chaser
#include "FastLED.h"
#include "Color.h"
#include "ProceduralColor.h"
#include "Button.h"
#include "Potentiometer.h"
#include "Logging.h"
#include "LightModes.h"
#include "MathExtra.h"
#include "Personality.h"

#define DATA_PIN 9
#define NUM_LEDS 12
#define MAX_BRIGHTNESS 100

float emotionDuration = 7; //s
float emotionStartTime = 0; //s
float emotionTransitionTime = 3; //s

CRGB leds[NUM_LEDS];
CRGB nextLeds[NUM_LEDS];

Emotion currentEmotion = HAPPY;
Emotion nextEmotion = SAD;

Button button1 = Button(2);
Button button2 = Button(3);
Button button3 = Button(4);
Button button4 = Button(5);
Button button5 = Button(6);

Potentiometer topLeft = Potentiometer(A5, 0, 1023);
Potentiometer topRight = Potentiometer(A4, 0, 1023);
Potentiometer bottomLeft = Potentiometer(A3, 0, 1023);
Potentiometer bottomMiddle = Potentiometer(A2, 0, 1023);
Potentiometer bottomRight = Potentiometer(A1, 0, 1023);

Emotion getNextEmotion(Emotion current) {
    switch (current) {
        case HAPPY: return SAD;
        case SAD: return ANGRY;
        case ANGRY: return PASSIONATE;
        case PASSIONATE: return HAPPY;
    }
}

void setup() {
    Log::connect();
    FastLED.setBrightness(MAX_BRIGHTNESS);
    CFastLED::addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
}

void loop() {
    float seconds = (float) millis() / 1000.f;


    // Transition completed
    if (seconds - emotionStartTime > emotionDuration + emotionTransitionTime) {
        currentEmotion = nextEmotion;
        nextEmotion = getNextEmotion(nextEmotion);
        emotionStartTime = seconds;
    }
    // Transition period
    else if (seconds - emotionStartTime > emotionDuration) {
        getColor(currentEmotion, seconds, leds, NUM_LEDS);
        getColor(nextEmotion, seconds, nextLeds, NUM_LEDS);
        auto transitionAmount = (fract8) ((seconds - emotionStartTime - emotionDuration) / emotionTransitionTime * 255);
        nblend(leds, nextLeds, NUM_LEDS, transitionAmount);
    }
    else {
        getColor(currentEmotion, seconds, leds, NUM_LEDS);
    }

    FastLED.show();
}
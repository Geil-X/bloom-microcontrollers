#include <Arduino.h>
#include <AccelStepper.h>
#include <TMCStepper.h>

#include "FastLED.h"
#include "Color.h"
#include "ProceduralColor.h"
#include "Button.h"
#include "Potentiometer.h"
#include "Logging.h"
//#include "LightModes.h"
#include "MathExtra.h"
#include "CircularGradient.h"

#define NUM_LEDS 12
#define MAX_BRIGHTNESS 100 // watch the power!
#define DATA_PIN 9
#define NUM_GRADIENT_COLORS 3

// This is an array of leds, one for each led in your ring
CRGB leds[NUM_LEDS];
CHSV newHsl;


// For circular color palettes
CircularColorNode *circularGradient[NUM_GRADIENT_COLORS] = {
        new CircularColorNode(CRGB::Yellow, 25),
        new CircularColorNode(CRGB::Blue, 120),
        new CircularColorNode(CRGB::Purple, 200),
};
Angle neoPixelLedAngles[NUM_LEDS];

enum ColorMode {
    HSV_MODE,
    CIRCULAR_COLOR_GRADIENT,
    LIGHTNING_MODE,
    FIREFLY_MODE,
    FIRE_CRACKLE_MODE,
};


ColorMode colorScheme = HSV_MODE;
CRGB color = CRGB(0, 0, 0);

Button hsvButton = Button(2);
Button colorGradientButton = Button(3);
Button lightningButton = Button(4);
Button fireflyButton = Button(5);
Button fireCrackleButton = Button(6);

Potentiometer topLeft = Potentiometer(A5, 0, 255);
Potentiometer topRight = Potentiometer(A4, 0, 255);
Potentiometer bottomLeft = Potentiometer(A3, 0, 255);
Potentiometer bottomMiddle = Potentiometer(A2, 0, 255);
Potentiometer bottomRight = Potentiometer(A1, 0, 255);

//NoisyGradient noisyGradientPreferences = {};


void setup() {
    Log::connect();
    Log::info("Starting Neo Pixel Controller");
    FastLED.setBrightness(MAX_BRIGHTNESS);
    CFastLED::addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);

//    noisyGradientPreferences = {
//            .backgroundColor = Color::Red,
//            .accentColor = Color::Green,
//            .flickerRate = 1,
//            .ledDistance = 1,
//            .flickerSharpness = 1,
//            .rotationsPerSecond = 0.f,
//            .jumpy = false,
//    };

//    circularGradient[0] = new CircularColorNode(Color::Magenta, 25);
//    circularGradient[1] = new CircularColorNode(Color::Yellow, 120);
//    circularGradient[2] = new CircularColorNode(Color::Cyan, 200);

    // Populate the angles for the 12 LED neo-pixel ring
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        neoPixelLedAngles[i] = qmul8(UINT8_MAX / NUM_LEDS, i);
    }
}

String hsvToString(const CHSV &hsv) {
    return "Hsv(" + String(hsv.h) + ", " + String(hsv.s) + ", " + String(hsv.v) + ")";
}


void loop() {
    float secs = (float) millis() / 1000.0f;

    if (hsvButton.isPressed()) colorScheme = HSV_MODE;
    if (colorGradientButton.isPressed()) colorScheme = CIRCULAR_COLOR_GRADIENT;
    if (lightningButton.isPressed()) colorScheme = LIGHTNING_MODE;
    if (fireflyButton.isPressed()) colorScheme = FIREFLY_MODE;
    if (fireCrackleButton.isPressed()) colorScheme = FIRE_CRACKLE_MODE;

    switch (colorScheme) {
        case HSV_MODE: {
            auto h = (uint8_t) bottomLeft.value();
            auto s = (uint8_t) bottomMiddle.value();
            auto v = (uint8_t) bottomRight.value();
            newHsl = CHSV(h, s, v);
            if (newHsl != color) {
                Serial.println(hsvToString(newHsl));
                color = newHsl;
            }
            FastLED.showColor(color);
            break;
        }
        case CIRCULAR_COLOR_GRADIENT: {
            Angle addedAngle = (uint8_t) bottomLeft.value();
            for (uint8_t i = 0; i < NUM_LEDS; i++) {
                leds[i] = colorAt(neoPixelLedAngles[i] + addedAngle, circularGradient, NUM_GRADIENT_COLORS);
            }
            break;
        }
        case LIGHTNING_MODE: {
//            lightning(secs, leds, NUM_LEDS);
            break;
        }
        case FIREFLY_MODE: {
//            fireflies(secs, leds, NUM_LEDS);
            break;
        }
        case FIRE_CRACKLE_MODE: {
//            fireCrackle(secs, leds, NUM_LEDS);
            break;
        }
    }

    FastLED.show();
}
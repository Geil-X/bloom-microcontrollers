#include <Arduino.h>
#include <Wire.h>
#include <Logging.h>
#include <Interrupt.h>
#include <Flower.h>
#include <MorningGlory.h>
#include <MathExtra.h>

#include "FastLED.h"
#include "ProceduralColor.h"

Flower flower = Flower(EN, DIR, STEP, SS, SDI, SDO, SCK);
VoltageSensor motorVoltage = VoltageSensor(VM_REF, VOLTAGE_RESISTOR_1, VOLTAGE_RESISTOR_2, MOTOR_VOLTAGE_THRESHOLD);

// Neopixel Led
#define DATA_PIN 6
#define NUM_LEDS 12
#define MAX_BRIGHTNESS 100 // watch the power!
CRGB leds[NUM_LEDS];

float period = 30.f;

// Springtime Bloom

CRGB purple(141, 15, 166);
CRGB blue(53, 177, 230);
CRGB green(168, 230, 53);
CRGB orange(230, 80, 25);

Gradient2D springTimeBloom(purple, blue, orange, green);

ISR(TIMER1_COMPA_vect) {
    flower.run();
}

void setupFlower() {
    Log::info("Running setup sequence");
    flower.setup();
    Log::info("Running home sequence");
    flower.home();
    flower.setMaxSpeed(1000);
    flower.setAcceleration(500);

    Log::info("Setting up `flower.run()` interrupts");
    setUpInterrupts(18);
}

void setup() {
    Log::connect();
    Log::info("Waiting for motor to gain power");
    while (!motorVoltage.hasPower()) {}
    Log::info("Motor has power");

    // Setup the Neopixel
    FastLED.setBrightness(MAX_BRIGHTNESS);
    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);

    setupFlower();
}

String rgbToString(const CRGB &rgb) {
    return "Xyz(" + String(rgb.r) + ", " + String(rgb.g) + ", " + String(rgb.b) + ")";
}

__attribute__((unused)) void loop() {
    if (flower.isOpen()) { flower.closeAsync(); }
    if (flower.isClosed()) { flower.openAsync(); }

    if (motorVoltage.lostPower()) {
        Log::info("Motor lost power");
    }

    if (motorVoltage.gainedPower()) {
        Log::info("Motor regained power");
        delay(500);
        setupFlower();
    }

    if (flower.motorStalled()) {
        delay(500);
        flower.home();
    }

    float s = (float) millis() / 1000.0f;
    auto circularTime = (float) fmod(s / period, 1.);
    circularTime = circularTime < 0.5 ? circularTime * 2 : 1 - ((circularTime - 0.5f) * 2);

    CRGB color = springTimeBloom.colorAt(circularTime, flower.getPosition());
    FastLED.showColor(color);
//    for (auto &led: leds) {
//        led = color;
//    }
//    FastLED.show();
}

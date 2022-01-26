#include <Arduino.h>
#include <Wire.h>
#include <Logging.h>
#include <Interrupt.h>
#include <Flower.h>
#include <MorningGlory.h>
#include <MathExtra.h>

#include "FastLED.h"
#include "Personality.h"

Flower flower = Flower(EN, DIR, STEP, SS, SDI, SDO, SCK);
VoltageSensor motorVoltage = VoltageSensor(VM_REF, VOLTAGE_RESISTOR_1, VOLTAGE_RESISTOR_2, MOTOR_VOLTAGE_THRESHOLD);

Excited *excited = new Excited();
Sleepy *sleepy = new Sleepy();
Scared *scared = new Scared();
Curious *curious = new Curious();

IEmotionState *currentEmotion = (IEmotionState *) sleepy;

// Neo Pixel Led
#define DATA_PIN 6
#define NUM_LEDS 12
#define MAX_BRIGHTNESS 100 // watch the power!
CRGB currentLeds[NUM_LEDS];
CRGB blendLeds[NUM_LEDS];

float emotionStartTime = 0.f;
float emotionDuration = 15.0f;
float emotionBlendTime = 5.f;

ISR(TIMER1_COMPA_vect) {
    flower.run();
}

void setupFlower() {
    Log::info("Running setup sequence");
    flower.setup();
    Log::info("Running home sequence");
    flower.home();

    Log::info("Setting up `flower.run()` interrupts");
    setUpInterrupts(18);

    currentEmotion->initFlower(flower);
}

void setup() {
    Log::connect();
    Log::info("Waiting for motor to gain power");
    while (!motorVoltage.hasPower()) {}
    Log::info("Motor has power");

    // Setup the Neopixel
    FastLED.setBrightness(MAX_BRIGHTNESS);
    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(currentLeds, NUM_LEDS);

    setupFlower();
    Log::info("Running in excited state");
}

String rgbToString(const CRGB &rgb) {
    return "Xyz(" + String(rgb.r) + ", " + String(rgb.g) + ", " + String(rgb.b) + ")";
}

void getNextEmotion() {
    switch (random8(4)) {
        case 0:
            Log::info("Running in excited state");
            currentEmotion = (IEmotionState *) excited;
            break;
        case 1:
            Log::info("Running in sleepy state");
            currentEmotion = (IEmotionState *) sleepy;
            break;
        case 2:
            Log::info("Running in scared state");
            currentEmotion = (IEmotionState *) scared;
            break;
        case 3:
            Log::info("Running in curious state");
            currentEmotion = (IEmotionState *) curious;
            break;
        default:
            Log::info("Running in excited state by default");
            currentEmotion = (IEmotionState *) excited;
            break;
    }

    currentEmotion->initFlower(flower);
}

void choreography() {
    currentEmotion->controlMotor(millis(), flower);
    currentEmotion->controlLights(millis(), flower, currentLeds, NUM_LEDS);
}

__attribute__((unused)) void loop() {
    float seconds = (float) millis() / 1000.f;

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

    if (seconds - emotionStartTime > emotionDuration) {
        getNextEmotion();
        emotionStartTime = seconds;
    }

    choreography();

    FastLED.show();
}

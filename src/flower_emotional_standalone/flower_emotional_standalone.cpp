#include <Arduino.h>
#include <Wire.h>
#include <FastLED.h>

#include <Logging.h>
#include <Interrupt.h>
#include <Flower.h>
#include <MorningGlory.h>
#include <Personality.h>
#include <EmotionController.h>
#include <LedIndicator.h>


// Emotional States
Excited *excited = new Excited();
Sleepy *sleepy = new Sleepy();
Angry *angry = new Angry();
Curious *curious = new Curious();

uint32_t emotionDuration = 10 * 1000; // ms
uint32_t emotionTransitionTime = 2 * 1000; // ms
EmotionController emotionController((IEmotionState *) excited);

// Controllers and Sensors
Flower flower = Flower(EN, DIR, STEP, SS, SDI, SDO, SCK);
VoltageSensor motorVoltage = VoltageSensor(VM_REF, VOLTAGE_RESISTOR_1, VOLTAGE_RESISTOR_2, MOTOR_VOLTAGE_THRESHOLD);
LedIndicator ledIndicator(IND_PIN);

// Neo Pixel Led
#define DATA_PIN 6
#define NUM_LEDS 12
#define MAX_BRIGHTNESS 100
CRGB leds[NUM_LEDS];

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

    emotionController.initFlower(flower);
}

void setup() {
    // Setup serial communication
    Log::connect();
    Log::info("Waiting for motor to gain power");

    // Wait until the program receives 12v motor input voltage
    ledIndicator.blink(250, 250);
    while (!motorVoltage.hasPower()) {
        ledIndicator.update();
    }
    Log::info("Motor has power");

    // Set up the neo-pixel
    FastLED.setBrightness(MAX_BRIGHTNESS);
    CFastLED::addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);

    // Run flower setup
    ledIndicator.blinkBlocking(50, 50, 5);
    ledIndicator.on();
    setupFlower();
    ledIndicator.blinkBlocking(50, 50, 5);

    ledIndicator.on();
}

IEmotionState* getNextEmotion() {
    switch (random8(4)) {
        case 0: return (IEmotionState *) excited;
        case 1: return (IEmotionState *) sleepy;
        case 2: return (IEmotionState *) angry;
        case 3: return (IEmotionState *) curious;
        default: return (IEmotionState *) excited;
    }
}

__attribute__((unused)) void loop() {
    uint32_t ms = millis();

    if (motorVoltage.lostPower()) {
        Log::info("Motor lost power");
        ledIndicator.blink(250, 250);
    }

    if (motorVoltage.gainedPower()) {
        Log::info("Motor regained power");
        ledIndicator.blinkBlocking(100, 100, 10);
        setupFlower();
    }

    if (flower.motorStalled()) {
        Log::info("Motor stalled");
        ledIndicator.blinkBlocking(50, 50, 20);
        flower.home();
    }

    if (!emotionController.isTransitioning() && emotionController.getEmotionDuration(ms) > emotionDuration) {
        emotionController.transitionTo(getNextEmotion(), emotionTransitionTime, ms);
    }

    // Update controllers
    emotionController.update(ms, flower, leds, NUM_LEDS);
    ledIndicator.update();

    // Display new LED state
    FastLED.show();
}

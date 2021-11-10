#include <Arduino.h>
#include <I2CPeripheral.h>
#include <Logging.h>
#include <DistanceSensor.h>
#include <Choreography.h>
#include <noise.h>

// Pin Definitions for different microcontrollers

#define NEW_COMMAND_TIME 10  // ms

// Stepper Pins
#define DIAG1_PIN   3
#define EN_PIN      7
#define DIR     8
#define STEP    9
#define CS      10

// Stepper Driver SPI Communication
#define MOSI    11  // SDI
#define MISO    12  // SDO
#define SCK     13  // SPI Reference Clock

// Sensor Pins
#define MOTOR_VOLTAGE A0

#define ECHO_PIN 6
#define TRIGGER_PIN 5
DistanceSensor distance_sensor(TRIGGER_PIN, ECHO_PIN);

#define EXCITED_DISTANCE 3.f // ft
#define EXCITED_DURATION 100 // ms

Flower flower = Flower(EN_PIN, DIR, STEP, CS, MOSI, MISO, SCK, DIAG1_PIN, MOTOR_VOLTAGE);
unsigned long last_command = 0;
unsigned long last_excited_time = 0;

/** Return the relative distance to an object. Distance is between 0 and 1 where 0 is close and 1 is far away. */
float relativeDistance() {
#define MAX_DISTANCE 3
#define MIN_DISTANCE 1
    float distance = constrain(distance_sensor.distanceFt(), MIN_DISTANCE, MAX_DISTANCE);
    return (distance - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE);
}

float wobble(float seconds) {
    float seconds_per_cycle = 5.f;
    float sinusoidal = ncos(seconds, 1.f / seconds_per_cycle);

    return sinusoidal;
}

float open(float t, int seed, float duration, float x, float y) {
    return 1.;
}

float close(float t, int seed, float duration, float x, float y) {
    return 0.;
}

void setupFlower() {
    Log::info("Running setup sequence");
    flower.setup();
    flower.home();
    flower.setMaxSpeed(10000);
    flower.setAcceleration(5000);
    Log::info("Running main loop");
}

void setup() {
    Log::connect(Log::DEBUG, 115200);
    Log::info("Running shy standalone flower");

    while (!flower.hasPower()) {}
    Log::info("Motor has power");
    setupFlower();
}

bool isExcited(unsigned long ms) {
    return (ms - last_excited_time) < EXCITED_DURATION;
}

void updatePosition(unsigned long ms) {
    if (distance_sensor.distanceFt(EXCITED_DISTANCE + 1) < EXCITED_DISTANCE) {
        last_excited_time = ms;
    }

    float seconds = (float) ms / 1000.f;
    float position;

    if (isExcited(ms)) {
        position = (1.f + wobble(seconds)) / 2.f;
    } else {
        position = wobble(seconds) / 3.f;
    }

    float mapped_position = map(position, 0, 1, 0, 100);
    Command::openTo(flower, mapped_position);
}

__attribute__((unused)) void loop() {
    unsigned long ms = millis();
    if (ms - last_command > NEW_COMMAND_TIME) {
        updatePosition(ms);
        last_command = ms;
    }

    if (flower.regainedPower()) {
        Log::info("Motor regained power");
        delay(500);
        setupFlower();
    }

//    if (flower.motorStalled()) {
//        delay(500);
//        flower.home();
//    }

    flower.run();
}

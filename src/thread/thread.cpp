//#include "TeensyThreads.h"

#include "Flower.h"
#include "FlowerGroup.h"

// Flower Defaults
const int SPEED = 5000;
const int ACCELERATION = 5000;
const int STALL_THRESHOLD = 8;
const int BOUNDARY_OFFSET = 40;
const int STALL_DETECTION_MOVE_BLOCK = 40;

// Execution Variables

Flower *flower1 = new Flower(2, 3, 22);
Flower *flower2 = new Flower(4, 5, 23);

void setup() {
    Serial.begin(9600);

    Serial.println("Setup Begun");

    FlowerGroup::init();
    FlowerGroup::addStepper(flower1);
    FlowerGroup::addStepper(flower2);
    FlowerGroup::setup();
    FlowerGroup::setThreshold(0, STALL_THRESHOLD, BOUNDARY_OFFSET, STALL_DETECTION_MOVE_BLOCK);
    FlowerGroup::setThreshold(1, STALL_THRESHOLD, BOUNDARY_OFFSET, STALL_DETECTION_MOVE_BLOCK);
    FlowerGroup::setSpeed(SPEED);
    FlowerGroup::setAcceleration(ACCELERATION);

    Serial.println("Setup Finished");
}

void loop() {
    int *first = new int[2]{1, 1};
    int *second = new int[2]{1, 2};

    Serial.println("Loop Motor");
    for (int i = 0; i < 100; i += 10) {
        Serial.println(String(i));
        FlowerGroup::move(i, first);
        FlowerGroup::move(i, second);
        delay(100);
    }
}

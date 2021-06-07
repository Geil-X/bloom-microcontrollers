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
FlowerGroup group;
int count = 0;
bool startCount = false;

void setup() {
    Serial.begin(9600);

    while (!Serial) {}

    Serial.println("Setup Begun");

    group.addStepper(new Flower(2, 3, 22));
    group.addStepper(new Flower(4, 5, 23));

    group.setup();
    group.setThreshold(0, STALL_THRESHOLD, BOUNDARY_OFFSET, STALL_DETECTION_MOVE_BLOCK);
    group.setThreshold(1, STALL_THRESHOLD, BOUNDARY_OFFSET, STALL_DETECTION_MOVE_BLOCK);
    group.setSpeed(5000);
    group.setAcceleration(5000);

    Serial.println("Setup Finished");
}

void loop() {
    int *first = new int[2]{1, 1};
    int *second = new int[2]{1, 2};

    Serial.println("Loop Motor");
    for (int i = 0; i < 100; i += 10) {
        Serial.println(String(i));
        group.move(i, first);
        group.move(i, second);
        delay(100);
    }
}

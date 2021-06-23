#include "Flower.h"
#include "TeensyStep.h"
#include "FlowerGroup.h"

Flower flower1(2, 3, A8);
Flower flower2(4, 5, A9);
FlowerGroup group;

void setup() {
    group.addStepper(&flower1);
    group.addStepper(&flower2);
    //group.addStepper(flower3);
    group.setup();
//    group.setThrehold(0,8,40,40); // int flower id, int stall_threhold, int boundry_offset, int stall_detection_move_block
    //group.moveAbs(0); // Move to Abs pos 0
    //group._isrunning = true;
}

void loop() {
    for (int i = 0; i < 105; i += 5) {
        group.move(i); //Move to i percent position
        delay(20);
    }
    for (int i = 100; i > 0; i -= 5) {
        group.move(i); //Move to i percent position
        delay(20);
    }
}
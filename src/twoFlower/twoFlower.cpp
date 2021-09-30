
#include "Flower.h"

Flower flower1(2, 3, 22);
Flower flower2(4, 5, 23);
int _count = 0;

void setup() {
    flower1.setup();
    flower1.home();
    flower1.setRate(2000, 2000);
    flower2.setup();
    flower2.home();
    flower2.setRate(2000, 2000);
}

void loop() {
    for (int i = 0; i < 100; i += 10) {
        flower1.open(i);
        delay(100);
    }
    for (int i = 0; i < 100; i += 10) {
        flower2.open(i);
        delay(100);
    }
}
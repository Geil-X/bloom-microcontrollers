
#include "Flower.h"

Flower flower(2, 3, 22);
int _count = 0;

void setup() {
    flower.setup();
    flower.home();
    flower.setRate(2000, 2000);

    flower.close();
    flower.open();
}

void loop() {
    flower.open();
    flower.close();
    for (int i = 0; i < 100; i += 10) {
        flower.open(i);
    }

    delay(1000);
    flower.open();
    flower.close();
    delay(1000);
}
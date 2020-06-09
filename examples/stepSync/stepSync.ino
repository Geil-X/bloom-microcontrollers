#include "Flower.h"


Flower flower1(2,3, A9);

void setup()
{
    flower1.setup();
    flower1.home();
    delay(1000);
    flower1.stepper.setPosition(0);
    Serial.println(flower1.stepper.getPosition());
    Serial.println(flower1.total_step);
    flower1.stepper.setTargetAbs(flower1.total_step);
    flower1.controller.move(flower1.stepper);
    
    Serial.println(flower1.stepper.getPosition());
    delay(1000);
    flower1.stepper.setTargetAbs(0);
    flower1.controller.move(flower1.stepper);

    Serial.println(flower1.stepper.getPosition());

}

void loop()
{
    Serial.println(flower1.stepper.getPosition());
}
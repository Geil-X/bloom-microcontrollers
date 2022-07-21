#include <Arduino.h>

#include <Types.h>
#include <Flower.h>
#include <I2cController.h>
#include <Logging.h>
#include <MathExtra.h>
#include <Set.h>

I2cController controller;
Set i2cAddresses;
int i2cAddress = -1;
unsigned int delayTime = 150;

void setup() {
    Log::connect(Log::DEBUG);
}

void loop() {
    controller.scan();
    i2cAddresses = controller.connectedAddresses();
    Percentage percentage = randomPercentage();
    delayTime = 1000 / i2cAddresses.count();

    i2cAddress = i2cAddresses.first();
    while (i2cAddress != -1) {
        controller.sendOpenToPacket(percentage, i2cAddress);
        delay(delayTime);
        i2cAddress = i2cAddresses.next();
    }
}

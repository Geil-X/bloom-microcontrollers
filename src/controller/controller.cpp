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
ResponsePacket responsePacket;
Position targetPosition;
Acceleration acceleration;


void setup() {
    Log::connect(Log::DEBUG);

    delay(5000);

    controller.scan();
    i2cAddresses = controller.connectedAddresses();


    i2cAddress = i2cAddresses.first();
    while (i2cAddress != -1) {
        controller.sendAccelerationPacket(100, i2cAddress);
        controller.sendSpeedPacket(250, i2cAddress);
        i2cAddress = i2cAddresses.next();
    }
}

void sendAllToPosition(Position position) {
    int i2c = i2cAddresses.first();
    while (i2c != -1) {
        controller.sendOpenToPacket(position, i2c);
        i2c = i2cAddresses.next();
    }
}

void loop() {
    controller.scan();
    i2cAddresses = controller.connectedAddresses();

    i2cAddress = i2cAddresses.first();
    while (i2cAddress != -1) {
        responsePacket = controller.request(i2cAddress);
        if (responsePacket.position == responsePacket.target) {
            targetPosition = randomPercentage();
            controller.sendOpenToPacket(targetPosition, i2cAddress);
            acceleration = random(500, 1000);
//            controller.sendAccelerationPacket(acceleration, i2cAddress);

            if (chance(5)) {
                sendAllToPosition(targetPosition);
            }
        }

        i2cAddress = i2cAddresses.next();
    }
    delay(50);
}

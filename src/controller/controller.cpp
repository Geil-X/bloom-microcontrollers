#include <Arduino.h>

#include <Types.h>
#include <Flower.h>
#include <I2cController.h>
#include <Logging.h>
#include <MathExtra.h>

I2cController controller;
ResponsePacket responsePacket;

void setup() {
    Log::connect(Log::DEBUG);
    Log::debug("Starting Serial TTL to I2C Converter");
}

void printResponsePacket() {
    Serial.println(responsePacket.time);
    Serial.println(responsePacket.position);
    Serial.println(responsePacket.target);
    Serial.println(responsePacket.acceleration);
    Serial.println(responsePacket.speed);
    Serial.println();
}

void loop() {
    controller.scan();
    Percentage percentage = randomPercentage();

    controller.sendOpenToPacket(percentage, 17);
    delay(250);
    responsePacket = controller.request(17);
    printResponsePacket();

    controller.sendOpenToPacket(percentage, 18);
    delay(250);
    responsePacket = controller.request(18);
    printResponsePacket();

    controller.sendOpenToPacket(percentage, 19);
    delay(250);
    responsePacket = controller.request(19);
    printResponsePacket();

    controller.sendOpenToPacket(percentage, 20);
    delay(250);
    responsePacket = controller.request(20);
    printResponsePacket();

    delay(3000);
}

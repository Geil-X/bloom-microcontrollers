#include <Arduino.h>

#include <I2CController.h>
#include <MathExtra.h>
#include <easing.h>
#include <Choreography.h>
//#include <sdnoise1234.h>

// Device Parameters
#define DEVICE_ID_1 16
#define DEVICE_ID_2 17
#define POLL_TIME 500 // ms

// Choreography parameters
#define SEQUENCE_TIME 60. // s

// Running Variables
#define SEQUENCE_COUNT 5
Choreography<SEQUENCE_COUNT> choreography;
unsigned long lastCommand = 0;


float springBloom(float t) {
    const uint8_t blooms = 6;
    return t * incos(t, blooms - 0.5);
}

float stayOpen(float t) {
    return 1;
}

float stayClosed(float t) {
    return 0;
}

float singleBloom(float t) {
    return ncos(t);
}

float close(float t) {
    return inverse(t);
}

void setup() {
    I2CController::join();
    choreography = Choreography<SEQUENCE_COUNT>()
            .addSequence(30, springBloom)
            .addSequence(3, stayOpen)
            .addSequence(10, singleBloom)
            .addSequence(20, close)
            .addSequence(3, stayClosed);

}

void sendCommand() {
    float position = map(choreography.perform(millis()), 0, 1, 0, 100);
    Packet positionPacket = I2CCommandFactory::openToPacket(position);
    Serial.println("Position: " + String(position));

    I2CController::sendPacket(DEVICE_ID_1, positionPacket);
    I2CController::sendPacket(DEVICE_ID_2, positionPacket);
}

void loop() {
    if (millis() - lastCommand > POLL_TIME) sendCommand();
}

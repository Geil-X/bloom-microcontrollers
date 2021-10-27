#include <Arduino.h>

#include <I2CController.h>
#include <MathExtra.h>
#include <DistanceSensor.h>
#include <Choreography.h>
#include <noise/noise1234.h>
#include <easing.h>

// Device Parameters
#define COMMAND_DELAY 5 // ms

#define SEQUENCE_COUNT 2
Choreography<SEQUENCE_COUNT> choreography;

#define DEVICE_COUNT 2
int devices[DEVICE_COUNT] = {16, 17};
unsigned long last_command;

#define DURATION 3.f

float open(int device, float t, float x, float y) {
    return 1.;
}

float close(int device, float t, float x, float y) {
    return 0.;
}

void setup() {
    Serial.begin(9600);
    I2CController::join();
    choreography = Choreography<SEQUENCE_COUNT>()
            .addSequence(DURATION, open)
            .addSequence(DURATION, close);
}

void sendCommand() {
    for (int device = 0; device < DEVICE_COUNT; device++) {
        float position = choreography.perform(device, millis(), 0., 0.);
        float motorPosition = map(position, 0, 1, 0, 100);
        Packet positionPacket = I2CCommandFactory::openToPacket(motorPosition);

        int device_id = devices[device];
        I2CController::sendPacket(device_id, positionPacket);
    }
}

void loop() {
    unsigned long milliseconds = millis();
    if (millis() - last_command > COMMAND_DELAY) {
        sendCommand();
        last_command = milliseconds;
    }
}

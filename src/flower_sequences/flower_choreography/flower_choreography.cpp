#include <Arduino.h>

#include <I2CController.h>
#include <MathExtra.h>
#include <Choreography.h>
#include <noise/noise1234.h>
#include <easing.h>

// Device Parameters
#define DEVICE_ID_1 16
#define DEVICE_ID_2 17
#define POLL_TIME 5 // ms

// Running Variables
#define SEQUENCE_COUNT 1
Choreography<SEQUENCE_COUNT> choreography;

#define DEVICE_COUNT 2
int devices[DEVICE_COUNT] = {16, 17};
unsigned long last_command;


#define DURATION 600.f

float wander(int device, float t, float x, float y) {
    float seconds_per_cycle = 5.f;
    float cycles = DURATION / seconds_per_cycle;
    float noise =  noise3(t * cycles, x, y);
    float normalized_noise =  (1 + noise) / 2;
    return easeInOutSine(normalized_noise);
}

void setup() {
    I2CController::join();
    choreography = Choreography<SEQUENCE_COUNT>()
            .addSequence(DURATION, wander);
}

void sendCommand() {
    for (int device = 0; device < DEVICE_COUNT; device++) {
        float position = choreography.perform(device, millis(), (float) device * 5.27f, (float) device * 6.87f);
        float motorPosition = map(position, 0, 1, 0, 100);
        Packet positionPacket = I2CCommandFactory::openToPacket(motorPosition);

        int device_id = devices[device];
        I2CController::sendPacket(device_id, positionPacket);
    }
}

void loop() {
    unsigned long milliseconds = millis();
    if (millis() - last_command > POLL_TIME) {
        sendCommand();
        last_command = milliseconds;
    }
}

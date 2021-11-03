#include <Arduino.h>

#include <I2CController.h>
#include <MathExtra.h>
#include <Choreography.h>
#include <noise.h>
#include <easing.h>
#include <Logging.h>

// Device Parameters
#define COMMAND_DELAY 5 // ms

#define SEQUENCE_COUNT 10
Choreography<SEQUENCE_COUNT> choreography;

#define DEVICE_COUNT 2
int devices[DEVICE_COUNT] = {16, 17};
unsigned long last_command;


/** Go from open state to wandering back to the open state. */
float wander(float t, int seed, float duration, float x, float y) {
    const float seconds_per_sync = 30.f;
    float shared_cycles = duration / seconds_per_sync;

    const float seconds_per_cycle = 4.f;
    float noise_cycles = duration / seconds_per_cycle;

    // Individual Wander
    float individual_noise = noise3d(t * noise_cycles, x, y, 3, .25, seed);
    float eased_noise = easeInOutBack(individual_noise);

    // Combined wander
    float shared_noise = noise3d(t * noise_cycles, x, y, 2, .25, seed);
    float shared_eased_noise = easeInOutSine(shared_noise);

    return lerp(ncos(t, shared_cycles), eased_noise, shared_eased_noise);
}

void setup() {
    Log::connect();
    Log::info("Running flower choreography");
    I2CController::join();
    choreography = Choreography<SEQUENCE_COUNT>()
            .addSequence(600.f, wander);
}

void sendCommand() {
    for (int device = 0; device < DEVICE_COUNT; device++) {
        int device_id = devices[device];
        float position = choreography.perform(millis(), (float) device * 4.23f, (float) device * 3.84f);
        float motorPosition = map(position, 0.f, 1.f, 0.f, 100.f);
        Packet positionPacket = I2CCommandFactory::openToPacket(motorPosition);
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

#include <Arduino.h>

#include <I2CController.h>
#include <Choreography.h>
#include <Logging.h>
#include <LedIndicator.h>

// Pin Listing
#define IND_PIN 14

// Device Parameters
#define COMMAND_DELAY 5 // ms

#define SEQUENCE_COUNT 10
Choreography<SEQUENCE_COUNT> choreography;

#define DEVICE_COUNT 2
int devices[DEVICE_COUNT] = {16, 17};
unsigned long last_command;
LedIndicator ledIndicator(IND_PIN);

void setup() {
    Log::connect();
    Log::info("Running flower choreography");
    I2CController::join();
    choreography = Choreography<SEQUENCE_COUNT>();
    ledIndicator.blinkBlocking(50, 50, 5);
    ledIndicator.blink(2000, 2000);
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

__attribute__((unused)) void loop() {
    unsigned long milliseconds = millis();
    if (millis() - last_command > COMMAND_DELAY) {
        sendCommand();
        last_command = milliseconds;
    }

    ledIndicator.update();
}

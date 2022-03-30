#include <Arduino.h>

#include <I2CController.h>
#include <Choreography.h>
#include <Logging.h>
#include <LedIndicator.h>

// Pin Listing
#define IND_PIN 14

// Device Parameters
#define COMMAND_DELAY 500 // ms


uint8_t devices[] = {7};
unsigned long last_command;
LedIndicator ledIndicator(IND_PIN);
I2CController i2CController;

void setup() {
    Log::connect();
    Log::info("Running flower choreography");
    ledIndicator.blinkBlocking(50, 50, 5);
    ledIndicator.blink(2000, 2000);
}

void sendCommand() {
    for (uint8_t &deviceId: devices) {
        i2CController.sendOpenToPacket(random16(), deviceId);
    }
}

__attribute__((unused)) void loop() {
    unsigned long milliseconds = millis();
    if (millis() - last_command > COMMAND_DELAY) {
        ledIndicator.off();
        sendCommand();
        last_command = milliseconds;
        ledIndicator.on();
    }
}

#include <Arduino.h>
#include <I2CTransfer.h>

struct Transmission {
    __attribute__((unused)) uint8_t buttonId = 0;
} transmission;

I2CTransfer transfer;

void processTransmission() {
    transfer.rxObj(transmission);
    Serial.println("Received data: " + String(transmission.buttonId));
}

// supplied as a reference - persistent allocation required
const functionPtr callbackArr[] = {processTransmission};

void setup() {
    Serial.begin(115200);
    Serial.println("Running I2C Peripheral Test");

    // I2C Configuration
    configST i2cConfig;
    i2cConfig.debug = true;
    i2cConfig.callbacks = callbackArr;
    i2cConfig.callbacksLen = sizeof(callbackArr) / sizeof(functionPtr);

    // Initialize I2C and data receiving protocols
    Wire.begin(16);
    transfer.begin(Wire, i2cConfig);
}

void loop() {
}
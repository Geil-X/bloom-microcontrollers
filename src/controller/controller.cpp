#include <Arduino.h>
#include <Wire.h>

void sendByte(unsigned char data) {
    Wire.beginTransmission(0);
    Wire.write(data);
    Wire.endTransmission();
}

unsigned char requestByte() {
    Wire.requestFrom(0, 1);
    if (!Wire.available()) return 255;
    return Wire.read();
}

void setup() {
    Serial.begin(9600);
    while (!Serial) {}
    Serial.println("Running...");

    Wire.begin();
}

void loop() {
    uint8_t data;
    for (uint8_t i = 0; i <= 255; i++) {
        delay(500);
        sendByte(i);
        data = requestByte();

        Serial.print(i);
        Serial.print(" ");
        Serial.println(data);
    }
}
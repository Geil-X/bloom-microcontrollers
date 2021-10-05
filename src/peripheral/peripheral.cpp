#include <Arduino.h>
#include <Wire.h>

unsigned char data;

void onReceive(int size) {
    while (Wire.available()) {
        data = Wire.read();
    }
}

void onRequest() {
    Wire.beginTransmission(1);
    Wire.write(data);
    Wire.endTransmission();
}

void setup() {
    Wire.begin(0);
    Wire.onReceive(onReceive);
    Wire.onRequest(onRequest);
}

void loop() {
}
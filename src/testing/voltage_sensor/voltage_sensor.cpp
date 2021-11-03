#include <Arduino.h>
#include <AccelStepper.h>
#include <TMCStepper.h>
#include <VoltageSensor.h>

#define VOLTAGE_PIN A0
#define R1 1000 // K ohms
#define R2 100 // K ohms


VoltageSensor voltage_sensor = VoltageSensor(VOLTAGE_PIN, R1, R2);
float voltage = 0;

void setup() {
    Serial.begin(9600);
    voltage_sensor = VoltageSensor(VOLTAGE_PIN, R1, R2);
}

void loop() {

    for (int i = 0; i < 10000; i++) {
        voltage = voltage_sensor.read();
    }
    Serial.println(voltage);
}
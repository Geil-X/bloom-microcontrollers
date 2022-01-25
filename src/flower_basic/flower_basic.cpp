#include <Arduino.h>
#include <Wire.h>
#include <Logging.h>
#include <Interrupt.h>
#include <Flower.h>
#include <MorningGlory.h>

Flower flower = Flower(EN, DIR, STEP, SS, SDI, SDO, SCK);
VoltageSensor motorVoltage = VoltageSensor(VM_REF, VOLTAGE_RESISTOR_1, VOLTAGE_RESISTOR_2, MOTOR_VOLTAGE_THRESHOLD);

ISR(TIMER1_COMPA_vect) {
    flower.run();
}

void setupFlower() {
    Log::info("Running setup sequence");
    flower.setup();
    Log::info("Running home sequence");
    flower.home();
    flower.setMaxSpeed(3000);
    flower.setAcceleration(2000);

    Log::info("Setting up `flower.run()` interrupts");
    setUpInterrupts(18);
}

void setup() {
    Log::connect();
    Log::info("Waiting for motor to gain power");
    while (!motorVoltage.hasPower()) {}
    Log::info("Motor has power");

    setupFlower();
}

__attribute__((unused)) void loop() {
    if (flower.isOpen()) { flower.closeAsync(); }
    if (flower.isClosed()) { flower.openAsync(); }

    if (motorVoltage.lostPower()) {
        Log::info("Motor lost power");
    }

    if (motorVoltage.gainedPower()) {
        Log::info("Motor regained power");
        delay(500);
        setupFlower();
    }

    if (flower.motorStalled()) {
        delay(500);
        flower.home();
    }
}

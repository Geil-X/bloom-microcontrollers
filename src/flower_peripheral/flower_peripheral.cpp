#include <Arduino.h>
#include <Wire.h>
#include <I2CPeripheral.h>
#include <DipSwitch.h>
#include <Logging.h>
#include <Interrupt.h>
#include <LedIndicator.h>
#include <MorningGlory.h>

int dip_pins[DIP_PIN_COUNT] = DIP_PINS;
DipSwitch<DIP_PIN_COUNT> dip_switch = DipSwitch<DIP_PIN_COUNT>(dip_pins);

Flower flower = Flower(EN, DIR, STEP, SS, SDI, SDO, SCK);
VoltageSensor motorVoltage = VoltageSensor(VM_REF, VOLTAGE_RESISTOR_1, VOLTAGE_RESISTOR_2, MOTOR_VOLTAGE_THRESHOLD);
LedIndicator ledIndicator(IND_PIN);

ISR(TIMER1_COMPA_vect) {
    flower.run();
}

void setupFlower() {
    Log::info("Running setup sequence");
    flower.setup();
    Log::info("Running home sequence");
    flower.home();
    flower.setMaxSpeed(10000);
    flower.setAcceleration(5000);

    Log::info("Setting up `flower.run()` interrupts");
    setUpInterrupts(18);
}

void setup() {
    Log::connect();
    Log::info("Booting up flower peripheral");
    uint8_t device_id = dip_switch.value() + 16;
    Log::info("Connecting to I2C on channel: " + String(device_id));
    I2CPeripheral::join(device_id);

    ledIndicator.blink(250, 250);
    while (!motorVoltage.hasPower()) {
        ledIndicator.update();
    }

    ledIndicator.blinkBlocking(50, 50, 5);
    ledIndicator.on();
    setupFlower();
    ledIndicator.blinkBlocking(50, 50, 5);

    ledIndicator.on();
}

__attribute__((unused)) void loop() {
    if (I2CPeripheral::executeCommand(flower)) {
        ledIndicator.blinkBlocking(1, 0, 1);
    }

    if (motorVoltage.lostPower()) {
        ledIndicator.blink(250, 250);
    }

    if (motorVoltage.gainedPower()) {
        ledIndicator.blink(2000, 2000);
        setupFlower();
    }

    if (flower.motorStalled()) {
        ledIndicator.blinkBlocking(50, 50, 20);
        flower.home();
    }

    ledIndicator.update();
}
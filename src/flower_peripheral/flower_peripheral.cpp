#include <Arduino.h>
#include <I2CPeripheral.h>
#include <DipSwitch.h>
#include <Logging.h>

// Pin Definitions for different microcontrollers

// Stepper Pins
#define DIAG1_PIN 3
#define EN_PIN    7
#define DIR       8
#define STEP      9
#define CS        10

// Stepper Driver SPI Communication
#define MOSI    11  // SDI
#define MISO    12  // SDO
#define SCK     13  // SPI Reference Clock

// Sensor Pins
#define MOTOR_VOLTAGE A0
#define DIP_PIN_COUNT 2
int dip_pins[DIP_PIN_COUNT] = {4, 5};

#define REFRESH_RATE 500 // us
unsigned long last_refresh = 0;

Flower flower = Flower(EN_PIN, DIR, STEP, CS, MOSI, MISO, SCK, DIAG1_PIN, MOTOR_VOLTAGE);
DipSwitch<DIP_PIN_COUNT> dip_switch = DipSwitch<DIP_PIN_COUNT>(dip_pins);

void setupFlower() {
    Log::info("Running Setup Sequence");
    flower.setup();
    flower.home();
    flower.setMaxSpeed(10000);
    flower.setAcceleration(5000);
}

void setup() {
    Log::connect();
    uint8_t device_id = dip_switch.value() + 16;
    Log::info("Connecting to I2C on channel: " + String(device_id));
    I2CPeripheral::join(device_id);

    while (!flower.hasPower()) {}
    Log::info("Motor has power");
    setupFlower();
}

__attribute__((unused)) void loop() {
    unsigned long ms = millis();
    if (ms  - last_refresh   > REFRESH_RATE) {
        I2CPeripheral::executeCommand(flower);

        if (flower.regainedPower()) {
            Log::info("Motor regained power");
            delay(500);
            setupFlower();
        }

//    if (flower.motorStalled()) {
//        delay(500);
//        flower.home();
//    }
    }


    flower.run();
}
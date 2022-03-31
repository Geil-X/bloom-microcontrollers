#include <Arduino.h>

#include <I2CPeripheral.h>
#include <DipSwitch.h>
#include <Logging.h>
#include <Interrupt.h>
#include <LedIndicator.h>
#include <MorningGlory.h>


// Peripheral Devices
int dip_pins[DIP_PIN_COUNT] = DIP_PINS;
DipSwitch<DIP_PIN_COUNT> dip_switch = DipSwitch<DIP_PIN_COUNT>(dip_pins);

Flower flower = Flower(EN, DIR, STEP, SS, SDI, SDO, SCK);
VoltageSensor motorVoltage = VoltageSensor(VM_REF, VOLTAGE_RESISTOR_1, VOLTAGE_RESISTOR_2, MOTOR_VOLTAGE_THRESHOLD);
LedIndicator ledIndicator(IND_PIN);

// Communication
I2CPeripheral peripheralCommunication;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-attributes"
ISR(TIMER1_COMPA_vect) {
    flower.run();
}
#pragma clang diagnostic pop

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
    // Setup serial communication
    Log::connect(Log::DEBUG);
    Log::info("Booting up flower peripheral");

    // Initialize I2C Communication
    uint8_t device_id = dip_switch.value();
    Log::info("Connecting to I2C on channel: " + String(device_id));
    peripheralCommunication.begin(device_id);

    // Wait until the program receives 12v motor input voltage
    Log::info("Waiting for motor power");
    ledIndicator.blink(250, 250);
    while (!motorVoltage.hasPower()) {
        ledIndicator.update();
    }
    Log::info("Motor has power");

    // Run flower setup
    ledIndicator.blinkBlocking(50, 50, 5);
    ledIndicator.on();
    setupFlower();
    ledIndicator.blinkBlocking(50, 50, 5);

    ledIndicator.blink(2000, 100);
    Log::info("Running main sequence waiting for commands");
}

__attribute__((unused)) void loop() {
    peripheralCommunication.executeCommand(flower);

    if (motorVoltage.lostPower()) {
        ledIndicator.blink(250, 250);
    }

    if (motorVoltage.gainedPower()) {
        ledIndicator.blink(2000, 2000);
        setupFlower();
        ledIndicator.blink(2000, 100);
    }

    if (flower.motorStalled()) {
        ledIndicator.blinkBlocking(50, 50, 20);
        flower.home();
        ledIndicator.blink(2000, 100);
    }

    ledIndicator.update();
}
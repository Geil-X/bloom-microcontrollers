#include <Arduino.h>

#include <DipSwitch.h>
#include <Logging.h>
#include <Flower.h>
#include <MorningGlory.h>
#include <Interrupt.h>
#include <LedIndicator.h>
#include <VoltageSensor.h>
#include <I2cPeripheral.h>


// Peripheral Devices
int dip_pins[DIP_PIN_COUNT] = DIP_PINS;
DipSwitch<DIP_PIN_COUNT> dip_switch = DipSwitch<DIP_PIN_COUNT>(dip_pins);

Flower flower = Flower(EN, DIR, STEP, SS, SDI, SDO, SCK);
VoltageSensor motorVoltage = VoltageSensor(VM_REF, VOLTAGE_RESISTOR_1, VOLTAGE_RESISTOR_2, MOTOR_VOLTAGE_THRESHOLD);
LedIndicator ledIndicator(IND_PIN);

// Communication
I2cPeripheral peripheralCommunication;


// TODO: Move to be inside the flower class
ISR(TIMER1_COMPA_vect) {
    flower.run();
}

void setupFlower() {
    Log::info("Running initial setup sequence");
    flower.setup();

    Log::info("Running initial home sequence");
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

    // Initialize I2c Communication
    uint8_t device_id = dip_switch.value();
    Log::info("Connecting to I2c on channel: " + String(device_id));
    peripheralCommunication.begin(device_id, &flower);

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

void loop() {

    if (motorVoltage.lostPower()) {
        Log::info("Motor lost power");
        ledIndicator.blink(250, 250);
    }

    if (motorVoltage.gainedPower()) {
        Log::info("Motor regained power");
        ledIndicator.blinkBlocking(50, 50, 5);
        flower.home();
        ledIndicator.blink(2000, 100);
    }

    if (flower.motorStalled()) {
        Log::info("Motor stalled");
        ledIndicator.blinkBlocking(50, 50, 20);
        flower.home();
        ledIndicator.blink(2000, 100);
    }

    peripheralCommunication.executeCommand();
    ledIndicator.update();
}
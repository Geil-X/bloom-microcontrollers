#ifndef FLOWER_MORNINGGLORY_H
#define FLOWER_MORNINGGLORY_H

#include <Arduino.h>

// Stepper Pins
#define EN   7  // PD7
#define DIR  8  // PB0
#define STEP 9  // PB1
#define SS   10 // PB2

#define SDI 11  // PB3
#define SDO 12  // PB4
#define SCK 13  // PB5

// Dip Switch

#define DIP_PIN_COUNT 7
#define DIP_PINS {2, 3, 4, 5, 6, 15, 16}

// Other

#define IND_PIN 14 // PC0

// Motor Voltage Sensor

#define VM_REF 17 // PC3
//     Resistors in the voltage dividers for measuring the motor voltage
#define VOLTAGE_RESISTOR_1 1000   // 1M Ohm
#define VOLTAGE_RESISTOR_2 100    // 100k Ohm
#define MOTOR_VOLTAGE_THRESHOLD 7 // volts

#endif //FLOWER_MORNINGGLORY_H

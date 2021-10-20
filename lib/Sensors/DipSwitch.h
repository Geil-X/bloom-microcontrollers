#ifndef FLOWER_DIPSWITCH_H
#define FLOWER_DIPSWITCH_H

#include <Arduino.h>

/**
 * Dip switches are read from right to left order generally with numbers
 * from 1 on, 1 being the least significant bit. The value corresponds to the
 * binary number created in the right to left reading order. This is generally
 * reversed from how ordinary binary numbers are coded.
 *
 * Example:
 *   Dip Switch: 1 2 3 4 5 6 7
 *   Pin States: 1 0 0 0 1 1 0
 *   Binary: 0110001
 *   Value: 49
 *
 * @tparam MAX_PINS The number of pins being used in the dip switch.
 */
template<size_t MAX_PINS>
class DipSwitch {
public:
    /**
     * Create a dip switch by specifying which pins control the switch. The pin
     * labeled 1 should go first and give the pins in increasing order from there.
     *
     * @param varg Supply the pins. There should be as many pins in the
     *             dip switch constructor as specified by template parameter.
     */
    explicit DipSwitch(const int pins[]);

    /**
     * Read the dip switch to get the encoded value
     * @return Integer encoded dip switch value.
     */
    uint8_t value() const;

private:
    int pins[MAX_PINS] = {0};
};

template<size_t MAX_PINS>
DipSwitch<MAX_PINS>::DipSwitch(const int pins[]) {
    for (int i = 0; i < MAX_PINS; i++) {
        this->pins[i] = pins[i];
        pinMode(pins[i], INPUT_PULLUP);
    }
}

template<size_t MAX_PINS>
uint8_t DipSwitch<MAX_PINS>::value() const {
    uint8_t encoding = 0;

    // Get all the readings from the pins and add them to the shifted previous
    // values. This must be done in reverse as the lower pins store the LSB and
    // the last pin stores the MSB which must be read first. The digital reading
    // is also 0 for pressed, so we need to invert that value.
    for (int i = MAX_PINS - 1; i >= 0; i--) {
        encoding = (encoding << 1) + !digitalRead(pins[i]);
    }

    return encoding;
}

#endif //FLOWER_DIPSWITCH_H

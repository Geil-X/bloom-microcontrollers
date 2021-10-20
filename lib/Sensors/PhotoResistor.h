#ifndef FLOWER_PHOTORESISTOR_H
#define FLOWER_PHOTORESISTOR_H

#include <Arduino.h>

/**
 * Photo resistor, photocell or light-dependent resitor (LDR). The resistance
 * decreases with increasing light intensity.
 */
class PhotoResistor {
public:
    explicit PhotoResistor(uint8_t pin);

    /**
     * @return Value of the analog read sensor in the range [0, 1023].
     */
    int read() const;

private:
    uint8_t pin;
};


#endif //FLOWER_PHOTORESISTOR_H

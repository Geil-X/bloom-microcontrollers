#ifndef FLOWER_POTENTIOMETERBUTTON_H
#define FLOWER_POTENTIOMETERBUTTON_H

#include <Arduino.h>
#include "Button.h"


class PotentiometerButton : public Button {
public:
    PotentiometerButton(uint8_t button_pin, uint8_t potentiometer_pin, int low, int high);

    /**
     * Get the adjusted value from the potentiometer pin.
     * @returns The adjusted value between @see low and @high set in the constructor.
     */
    int value() const;

private:
    uint8_t potentiometer_pin;
    int low;
    int high;
};


#endif //FLOWER_POTENTIOMETERBUTTON_H

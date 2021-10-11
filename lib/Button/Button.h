#ifndef FLOWER_BUTTON_H
#define FLOWER_BUTTON_H

#include <Arduino.h>

enum ButtonState : uint8_t {
    PRESSED = LOW,
    RELEASED = HIGH
};

class Button {
public:
    /**
     * Create a button object tied to a particular pin. This library assumes
     * that the button is wired up between the pin and the ground using the
     * internal pullup resistor within the microcontroller.
     *
     * @param pin The pin to attach the button object to.
     */
    explicit Button(uint8_t pin);

    /**
     * Tells you if the button has been pressed.
     *
     * @note In order to not have an `update` function, this function serves
     * as the both the update function and the event state function. This should
     * be called as often as possible to maintain consistent results.
     *
     * @returns True when the button has been pressed.
     */
    bool isPressed();

private:
    unsigned long last_debounce_time = 0;
    ButtonState button_state = RELEASED;
    ButtonState last_button_state = PRESSED;
    uint8_t pin;
};

#endif //FLOWER_BUTTON_H
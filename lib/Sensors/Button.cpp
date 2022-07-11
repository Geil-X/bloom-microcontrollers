#include <Button.h>

// The amount of time to wait for button debouncing noise to go away
#define DEBOUNCE_DELAY 50 // ms

Button::Button(uint8_t pin) {
    this->pin = pin;

    pinMode(pin, INPUT_PULLUP);
}

bool Button::isPressed() {
    auto reading = (ButtonState) digitalRead(pin);
    bool debounced = false;
    if (reading != last_button_state) {
        last_debounce_time = millis();
    }

    // If the state has lasted for the debounce buffer time and new state is reached
    if ((millis() - last_debounce_time) > DEBOUNCE_DELAY && button_state != reading) {
        button_state = reading;
        debounced = button_state == RELEASED;
    }

    last_button_state = reading;
    return debounced;
}
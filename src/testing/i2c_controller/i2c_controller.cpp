#include <Arduino.h>
#include <MathExtra.h>
#include <I2CTransfer.h>
#include <Button.h>

struct Transmission {
    __attribute__((unused)) uint8_t buttonId = 0;
} transmission;

I2CTransfer transfer;

Button button1 = Button(2);
Button button2 = Button(3);
Button button3 = Button(4);
Button button4 = Button(5);
Button button5 = Button(6);


void setup() {
    Serial.begin(115200);
    Wire.begin();
    transfer.begin(Wire);

    Serial.println("Running I2C Controller Test");
}

void testButton(Button&button, uint8_t value) {
    if (button.isPressed()) {
        transmission.buttonId = value;
        transfer.sendDatum(transmission, 0, 16);
        Serial.println("Button Pressed: " + String(value));
    }
}

void loop() {
    testButton(button1, 1);
    testButton(button2, 2);
    testButton(button3, 3);
    testButton(button4, 4);
    testButton(button5, 5);
}
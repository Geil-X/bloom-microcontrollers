#include <Arduino.h>
#include <AccelStepper.h>
#include <TMCStepper.h>
#include <MathExtra.h>
#include <Potentiometer.h>
#include <Button.h>

Button button1 = Button(2);
Button button2 = Button(3);
Button button3 = Button(4);
Button button4 = Button(5);
Button button5 = Button(6);

Potentiometer pot1 = Potentiometer(A5, 0, 100);
Potentiometer pot2 = Potentiometer(A4, 0, 100);
Potentiometer pot3 = Potentiometer(A3, 0, 100);
Potentiometer pot4 = Potentiometer(A2, 0, 100);
Potentiometer pot5 = Potentiometer(A1, 0, 100);

void setup() {
    Serial.begin(115200);
}

void checkButton(const String &number, Button &button, Potentiometer &pot) {
    pot.value();
    if (button.isPressed()) {
        Serial.println("Button " + number + ": " + String(pot.value()));
    }
}

void loop() {
    checkButton("1", button1, pot1);
    checkButton("2", button2, pot2);
    checkButton("3", button3, pot3);
    checkButton("4", button4, pot4);
    checkButton("5", button5, pot5);
}
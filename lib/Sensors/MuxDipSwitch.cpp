#include <MuxDipSwitch.h>

MuxDipSwitch::MuxDipSwitch(uint8_t output, uint8_t select1, uint8_t select2, uint8_t select3) {
    this->output = output;
    this->select1 = select1;
    this->select2 = select2;
    this->select3 = select3;

    pinMode(output, INPUT_PULLUP);
    pinMode(select1, OUTPUT);
    pinMode(select2, OUTPUT);
    pinMode(select3, OUTPUT);
}


uint8_t MuxDipSwitch::read() {
#define DIP_SWITCH_PINS 7
    uint8_t encoding = 0;

    for (uint8_t dip_pin = 0; dip_pin < DIP_SWITCH_PINS; dip_pin++) {
        digitalWrite(select1, dip_pin & 0b1);
        digitalWrite(select2, dip_pin & 0b10);
        digitalWrite(select3, dip_pin & 0b100);
        delayMicroseconds(10);
        encoding = (encoding << 1) + !digitalRead(output);
    }

    return encoding;
}
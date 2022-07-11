#include <Arduino.h>
#include <Wire.h>

const byte MY_ADDRESS = 17;

// various commands we might get

enum {
    CMD_ID = 1,
    CMD_READ_A0 = 2,
    CMD_READ_D8 = 3
};

char command;

void sendSensor(const byte which) {
    int val = analogRead(which);
    byte buf[2];

    buf[0] = val >> 8;
    buf[1] = val & 0xFF;
    Wire.write(buf, 2);
}  // end of sendSensor

void receiveEvent(int howMany) {
    command = (char) Wire.read();  // remember command for when we get request
} // end of receiveEvent

void requestEvent() {
    switch (command) {
        case CMD_ID:
            Wire.write(0x55);
            break;   // send our ID
        case CMD_READ_A0:
            sendSensor(A0);
            break;  // send A0 value
        case CMD_READ_D8:
            Wire.write(digitalRead(8));
            break;   // send D8 value
        default:
            break;
    }  // end of switch

}  // end of requestEvent

void setup() {
    command = 0;

    pinMode(8, INPUT);
    digitalWrite(8, HIGH);  // enable pull-up
    pinMode(A0, INPUT);
    digitalWrite(A0, LOW);  // disable pull-up

    Wire.begin(MY_ADDRESS);
    Wire.onReceive(receiveEvent);  // interrupt handler for incoming messages
    Wire.onRequest(requestEvent);  // interrupt handler for when data is wanted
}  // end of setup


void loop() {
    // all done by interrupts
}  // end of loop
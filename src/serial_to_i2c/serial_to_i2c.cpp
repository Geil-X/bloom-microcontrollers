#include <Arduino.h>
#include <CommandPacket.h>

#define CONTROLLER_PACKET_SIZE 4
struct ControllerPacket {
    union {
        // Parameter access
        struct {
            uint8_t i2cAddress;
            Command::Id commandId;

            // Aliases for available packet types
            union {
                // "Open" & "Open To" Commands
                fract16 percentage;

                // Set Speed
                uint16_t speed;

                // Set Acceleration
                uint16_t acceleration;
            };
        };

        // Byte array access. This length needs to be updates as fields are added
        uint8_t arr[CONTROLLER_PACKET_SIZE];
    };
} controllerPacket;

bool printPacket() {
    switch (controllerPacket.commandId) {
        case Command::NO_COMMAND:
            Serial.println("No Command to address: " + String(controllerPacket.i2cAddress));
            return false;
        case Command::SETUP: {
            Serial.println("Setup to address: " + String(controllerPacket.i2cAddress));
            break;
        }
        case Command::HOME: {
            Serial.println("Home to address: " + String(controllerPacket.i2cAddress));
            break;
        }
        case Command::OPEN: {
            Serial.println("Open to address: " + String(controllerPacket.i2cAddress));
            break;
        }
        case Command::CLOSE: {
            Serial.println("Close to address:" + String(controllerPacket.i2cAddress));
            break;
        }
        case Command::OPEN_TO: {
            Serial.println(
                    "Open To: " + String(controllerPacket.percentage) + " to address: " +
                    String(controllerPacket.i2cAddress));
            break;
        }
        case Command::SPEED: {
            Serial.println(
                    "Speed: " + String(controllerPacket.speed) + " to address: " + String(controllerPacket.i2cAddress));
            break;
        }
        case Command::ACCELERATION: {
            Serial.println(
                    "Acceleration: " + String(controllerPacket.acceleration) + " to address: " +
                    String(controllerPacket.i2cAddress));
            break;
        }
        default:
            Serial.println("Unknown command at " + String(controllerPacket.i2cAddress));
            return false;
    }
    return true;
}

void setup() {
    Serial.begin(115200);
    while (!Serial) {}
}

void loop() {
}

__attribute__((unused)) void serialEvent() {
    if (!Serial.available()) return;
    Serial.readBytes(controllerPacket.arr, CONTROLLER_PACKET_SIZE);
    printPacket();
}
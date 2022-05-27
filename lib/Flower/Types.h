#ifndef FLOWER_TYPES_H
#define FLOWER_TYPES_H

// FastLED contains several type definitions
// typedef fract16 uint16_t
#include <FastLED.h>

// Toggle between the two modes of operation
// If both are enabled, OPEN_CLOCKWISE wins
//
// #define OPEN_CLOCKWISE
#define OPEN_COUNTERCLOCKWISE

typedef uint32_t Time;
typedef fract16 Position;
typedef fract16 Percentage;
typedef uint16_t Acceleration;
typedef uint16_t Speed;
typedef uint8_t PinIndex;
typedef uint8_t I2cAddress;
typedef uint16_t Steps;
typedef uint16_t StallLevel;
typedef int8_t StallTuning;

// The type definitions for Direction are conditional on OPEN_[COUNTER]CLOCKWISE
enum Direction {
    DIRECTION_CLOCKWISE,
    DIRECTION_COUNTERCLOCKWISE,
#if defined(OPEN_CLOCKWISE)
    // For clockwise open
    DIRECTION_OPEN = DIRECTION_CLOCKWISE,
    DIRECTION_CLOSE = DIRECTION_COUNTERCLOCKWISE
#elif defined(OPEN_COUNTERCLOCKWISE)
    // For counterclockwise open
    DIRECTION_CLOSE = DIRECTION_CLOCKWISE,
    DIRECTION_OPEN = DIRECTION_COUNTERCLOCKWISE
#endif
};
#include <Arduino.h>

namespace Command {
    enum Id : unsigned char {
        NO_COMMAND,
        SETUP,
        HOME,
        OPEN,
        CLOSE,
        OPEN_TO,
        SPEED,
        ACCELERATION,
    };
}

#define COMMAND_PACKET_SIZE 3

struct CommandPacket {
    union {
        struct {
            Command::Id commandId;
            union {
                // "Open" & "Open To" Commands
                Percentage percentage;

                // Set Speed
                Speed speed;

                // Set Acceleration
                Acceleration acceleration;
            };
        };
        uint8_t arr[COMMAND_PACKET_SIZE];
    };
};

#define RESPONSE_PACKET_SIZE 12

struct ResponsePacket {
    union {
        struct {
            Time time;
            Position position;
            Position target;
            Acceleration acceleration;
            Speed speed;
        };
        uint8_t arr[RESPONSE_PACKET_SIZE];
    };
};

#endif //FLOWER_TYPES_H

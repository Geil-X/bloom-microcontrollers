#ifndef FLOWER_SPRING_SEQUENCE_H
#define FLOWER_SPRING_SEQUENCE_H

#import <Arduino.h>
#import "../Choreography/Choreography.h"
#import "../Math/MathExtra.h"
#import "../Math/noise/sdnoise1234.h"

float springBloom(int device, float t, float x, float y) {
    const uint8_t blooms = 6;
    return t * incos(t, blooms - 0.5);
}

float stayOpen(int device, float t, float x, float y) {
    return 1;
}

float stayClosed(int device, float t, float x, float y) {
    return 0;
}

float singleBloom(int device, float t, float x, float y) {
    return ncos(t);
}

float wither(int device, float t, float x, float y) {
    return inverse(t) + (sdnoise1(t * 10, nullptr) / 7.f);
}

Choreography<6> spring_sequence = Choreography<6>()
            .addSequence(45, springBloom)
            .addSequence(2, stayOpen)
            .addSequence(20, singleBloom)
            .addSequence(3, stayOpen)
            .addSequence(20, wither)
            .addSequence(10, stayClosed);

#endif //FLOWER_SPRING_SEQUENCE_H

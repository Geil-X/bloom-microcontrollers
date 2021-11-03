#include <math.h>
#include "noise.h"
#include "noise/noise1234.h"

float noise(float x, int octaves, float frequency, int seed) {
    float value = (1 + noise1(x + (float) seed)) / 2;
    for (int octave = 1; octave < octaves; octave++) {
        value = (1 - frequency) * value + frequency * ((1 + noise1(x + (float) seed)) / 2);
    }
    return value;
}

float noise3d(float x, float y, float z, int octaves, float frequency, float persistence, int seed) {
    float value = (1 + noise3(x + (float) seed, y, z)) / 2;
    for (int octave = 1; octave < octaves; octave++) {
        auto local_frequency = (float) pow(frequency, octave);
        auto local_persistence = (float) pow(persistence, octave);
        value = (1 - frequency) * value +
                local_persistence *
                ((1 + noise3(local_frequency * x + (float) seed, y, z)) / 2);
    }
    return value;
}
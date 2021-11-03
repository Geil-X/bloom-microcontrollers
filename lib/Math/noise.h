#ifndef FLOWER_NOISE_H
#define FLOWER_NOISE_H


/** Return a value from 0 to 1. */
float noise(float x, int octaves = 1, float frequency = 0.25, int seed = 0);

/** Return a value from 0 to 1. */
float noise3d(float x, float y, float z, int octaves = 1, float frequency = 0.25, float persistence = -.25, int seed = 0);

#endif //FLOWER_NOISE_H

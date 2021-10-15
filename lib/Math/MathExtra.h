#ifndef FLOWER_MATHEXTRA_H
#define FLOWER_MATHEXTRA_H

float map(float amt, float from_low, float from_high, float to_low, float to_high);

float constant(float x);

float identity(float x);

float inverse(float x);

/**
 * Normalized cosine function with adjustable period. The function is
 * normalized by remapping the typical output from -1 to 1 into the range of
 * 0 to 1. The function also has an adjustable period with which the cosine
 * function oscillates.
 *
 * @param x Input value
 * @param period The period the adjust the output with.
 * @return Normalized cosine function with results between 0 and 1 inclusive.
 */
float ncos(float x, float period = 1.);

/**
 * Normalized sine function with adjustable period. The function is
 * normalized by remapping the typical output from -1 to 1 into the range of
 * 0 to 1. The function also has an adjustable period with which the sine
 * function oscillates.
 *
 * @param x Input value
 * @param period The period the adjust the output with.
 * @return Normalized cosine function with results between 0 and 1 inclusive.
 */
float nsin(float x, float period = 1.);

/**
 * Linear ramping function. If the input value is less than the start value
 * return 0. If the input value is greater than the stop value return 1.
 * If the input value is between the start and stop value, the value is the
 * linearly interpolated value between 0 and 1 for that interval.
 *
 * @note
 *     0 ___/‾‾‾‾‾‾ 1
 *
 * @param x  Input value.
 * @param start The value to start ramping up.
 * @param stop The value to complete ramping.
 * @return Ramped value from 0 to 1.
 */
float linearRamp(float x, float start, float stop);

#endif //FLOWER_MATHEXTRA_H

/**
 * Easing functions. All of the functions work on the domain and range from
 * 0 to 1. This makes the functions easily composable for modifying other
 * actions that are performed that need som slight modification.
 *
 * If you are looking for a good visual reference of how all of these functions
 * behave, the following link does a good job explaining them.
 *     https://easings.net/
 *
 * Code adapted from:
 *     https://github.com/nicolausYes/easing-functions
 */
#ifndef FLOWER_MATH_EASING
#define FLOWER_MATH_EASING

float linearWindow(float t, float windowing_percentage);
float easeInSine(float t);
float easeOutSine(float t);
float easeInOutSine(float t);
float easeInQuad(float t);
float easeOutQuad(float t);
float easeInOutQuad(float t);
float easeInCubic(float t);
float easeOutCubic(float t);
float easeInOutCubic(float t);
float easeInQuart(float t);
float easeOutQuart(float t);
float easeInOutQuart(float t);
float easeInQuint(float t);
float easeOutQuint(float t);
float easeInOutQuint(float t);
float easeInExpo(float t);
float easeOutExpo(float t);
float easeInOutExpo(float t);
float easeInCirc(float t);
float easeOutCirc(float t);
float easeInOutCirc(float t);
float easeInBack(float t);
float easeOutBack(float t);
float easeInOutBack(float t);
float easeInElastic(float t);
float easeOutElastic(float t);
float easeInOutElastic(float t);
float easeInBounce(float t);
float easeOutBounce(float t);
float easeInOutBounce(float t);

#endif // FLOWER_MATH_EASING


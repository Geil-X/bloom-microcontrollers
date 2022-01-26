#ifndef FLOWER_COLOR_GRADIENT_TEST_H
#define FLOWER_COLOR_GRADIENT_TEST_H

#include <unity.h>
#include <FastLED.h>

#include <CircularGradient.h>

CRGB color1 = CRGB(0, 0, 0);
CRGB color2 = CRGB(100, 100, 100);
CRGB color3 = CRGB(200, 200, 200);

#define NUM_COLORS 3

CircularColorNode *circularGradient[NUM_COLORS] = {
        new CircularColorNode(color1, 50),
        new CircularColorNode(color2, 100),
        new CircularColorNode(color3, 205)};

void test_assert_equal_crgb(CRGB expected, CRGB actual) {
//    Serial.println(String(expected.r) + "  :  " + String(actual.r));
//    Serial.println(String(expected.g) + "  :  " + String(actual.g));
//    Serial.println(String(expected.b) + "  :  " + String(actual.b));
    TEST_ASSERT_EQUAL_UINT8(expected.r, actual.r);
    TEST_ASSERT_EQUAL_UINT8(expected.g, actual.g);
    TEST_ASSERT_EQUAL_UINT8(expected.b, actual.b);
}

void test_angle_on_color() {
    CRGB actual1 = colorAt(50, circularGradient, NUM_COLORS);
    CRGB actual2 = colorAt(100, circularGradient, NUM_COLORS);
    CRGB actual3 = colorAt(205, circularGradient, NUM_COLORS);
    test_assert_equal_crgb(color1, actual1);
    test_assert_equal_crgb(color2, actual2);
    test_assert_equal_crgb(color3, actual3);
}

void test_color_normal() {
    CRGB actual1 = colorAt(75, circularGradient, NUM_COLORS);
    CRGB actual2 = colorAt(125, circularGradient, NUM_COLORS);
    CRGB actual3 = colorAt(175, circularGradient, NUM_COLORS);
    test_assert_equal_crgb(CRGB(50, 50, 50), actual1);
    test_assert_equal_crgb(CRGB(124, 124, 124), actual2);
    test_assert_equal_crgb(CRGB(171, 171, 171), actual3);
}

void test_color_low() {
    CRGB actual = colorAt(0, circularGradient, NUM_COLORS);
    test_assert_equal_crgb(CRGB(100, 100, 100), actual);
}

void test_color_high() {
    CRGB actual = colorAt(255, circularGradient, NUM_COLORS);
    test_assert_equal_crgb(CRGB(100, 100, 100), actual);
}

void color_gradient_test() {
    RUN_TEST(test_angle_on_color);
    RUN_TEST(test_color_normal);
    RUN_TEST(test_color_low);
    RUN_TEST(test_color_high);
}

#endif //FLOWER_COLOR_GRADIENT_TEST_H

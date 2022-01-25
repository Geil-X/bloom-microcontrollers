#ifndef FLOWER_ANGLE_TEST_H
#define FLOWER_ANGLE_TEST_H

#include <unity.h>

#include <Angle.h>

void test_shortest_distance() {
    Angle first = 10;
    Angle second = 50;
    Angle actual = first.shortestDistance(second);
    TEST_ASSERT_EQUAL_UINT8(40, actual.theta);
}

void test_shortest_distance_edge_case() {
    Angle first = 10;
    Angle second = 200;
    Angle actual = first.shortestDistance(second);
    TEST_ASSERT_EQUAL_UINT8(65, actual.theta);
}

void test_longest_distance() {
    Angle first = 10;
    Angle second = 50;
    Angle actual = first.longestDistance(second);
    TEST_ASSERT_EQUAL_UINT8(215, actual.theta);
}

void test_longest_distance_edge_case() {
    Angle first = 10;
    Angle second = 200;
    Angle actual = first.longestDistance(second);
    TEST_ASSERT_EQUAL_UINT8(190, actual.theta);
}

void angle_test() {
    RUN_TEST(test_shortest_distance);
    RUN_TEST(test_shortest_distance_edge_case);
    RUN_TEST(test_longest_distance);
    RUN_TEST(test_longest_distance_edge_case);
}

#endif //FLOWER_ANGLE_TEST_H

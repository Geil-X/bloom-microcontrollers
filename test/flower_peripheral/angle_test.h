#ifndef FLOWER_ANGLE_TEST_H
#define FLOWER_ANGLE_TEST_H

#include <unity.h>
#include <Angle.h>

void test_shortest_distance() {
    TEST_ASSERT_EQUAL_UINT8(40, minDelta8(10, 50));
}

void test_shortest_distance_edge_case() {
    TEST_ASSERT_EQUAL_UINT8(65, minDelta8(10, 200));
}

void test_longest_distance() {
    TEST_ASSERT_EQUAL_UINT8(215, maxDelta8(10, 50));
}

void test_longest_distance_edge_case() {
    TEST_ASSERT_EQUAL_UINT8(190, maxDelta8(10, 200));
}

void angle_test() {
    RUN_TEST(test_shortest_distance);
    RUN_TEST(test_shortest_distance_edge_case);
    RUN_TEST(test_longest_distance);
    RUN_TEST(test_longest_distance_edge_case);
}

#endif //FLOWER_ANGLE_TEST_H

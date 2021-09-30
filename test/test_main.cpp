#include <unity.h>
#include <Command.h>

// Test Setup and Tear Down

Command *command;
Command *expected;

void setUp(void) {
}

void tearDown(void) {
    delete command;
    delete expected;
}


// Test Suite

void test_equality() {
    command = new NoCommand();
    expected = new NoCommand();
    TEST_ASSERT_TRUE(*command == *expected)
}

// Arduino Test Program

void setup() {
    UNITY_BEGIN();

    RUN_TEST(test_equality);

    UNITY_END(); // stop unit testing
}

void loop() {
}
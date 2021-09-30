#include <Arduino.h>
#include "unity.h"

#include "group_comm/Commands/Command.h"
#include "group_comm/Commands/CommandFactory.h"

// Test Setup and Tear Down

Command *command;
Command *expected;
CommandFactory commandFactory;

void setUp(void) {
    commandFactory = CommandFactory();
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
    delay(2000); // service delay
    UNITY_BEGIN();

    RUN_TEST(test_equality);

    UNITY_END(); // stop unit testing
}

void loop() {
}
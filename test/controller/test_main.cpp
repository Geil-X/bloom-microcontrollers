/**
 * Unit testing module for the I2C controller interface.
 */
#include <unity.h>

#include <Command.h>
#include <I2CController.h>

#define PERIPHERAL_ID 16
#define COMMAND_COUNT 14

// Test Setup and Tear Down

Command *expected;
Command *actual;

Command *test_commands[COMMAND_COUNT] =
        {new NoCommand(),
         new Setup(),
         new Home(),
         new Open(),
         new Close(),
         new OpenTo(10),
         new OpenTo(50),
         new OpenTo(75),
         new Speed(100),
         new Speed(30000),
         new Acceleration(100),
         new Acceleration(5000),
         new Ping(),
         new InvalidCommand()};

// Test Suite

void test_equality() {
    actual = new NoCommand();
    expected = new NoCommand();

    TEST_ASSERT_TRUE(*actual == *expected)
}

void test_inequality() {
    actual = new NoCommand();
    expected = new NoCommand();

    TEST_ASSERT_FALSE(*actual != *expected)
}

void test_convert_commands() {
    for (int i = 0; i < COMMAND_COUNT; i++) {
        expected = test_commands[i];
        actual = I2CCommandFactory::parsePacket(I2CCommandFactory::createPacket(expected));

        TEST_ASSERT_TRUE(*actual == *expected)
    }

}

void test_receive_no_command() {
    expected = new NoCommand();
    actual = I2CController::requestCommand(PERIPHERAL_ID);

    TEST_ASSERT_TRUE(*actual == *expected)
}

void test_receive_sent_command() {
    for (int i = 0; i < COMMAND_COUNT; i++) {
        expected = test_commands[i];
        I2CController::sendCommand(PERIPHERAL_ID, expected);
        actual = I2CController::requestCommand(PERIPHERAL_ID);

        TEST_ASSERT_TRUE(*actual == *expected)
    }
}

// Arduino Test Program

int main() {
    I2CController::join();

    UNITY_BEGIN();

    RUN_TEST(test_equality);
    RUN_TEST(test_inequality);
    RUN_TEST(test_convert_commands);
//    RUN_TEST(test_receive_no_command);
//    RUN_TEST(test_receive_sent_command);

    UNITY_END(); // stop unit testing
}
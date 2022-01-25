#ifndef FLOWER_DIP_SWITCH_TEST_H
#define FLOWER_DIP_SWITCH_TEST_H

#include <unity.h>
#include <DipSwitch.h>

#define DIP_PIN_COUNT 7

int dip_pins[DIP_PIN_COUNT] = {2, 3, 4, 5, 6, 15, 16};
DipSwitch<DIP_PIN_COUNT> dip_switch = DipSwitch<DIP_PIN_COUNT>(dip_pins);

void test_dip_switch() {
    uint8_t expected =  (1 << DIP_PIN_COUNT) - 1;
    TEST_ASSERT_EQUAL_INT8(expected, dip_switch.value());
}

void dip_switch_test() {
    RUN_TEST(test_dip_switch);
}

#endif //FLOWER_DIP_SWITCH_TEST_H

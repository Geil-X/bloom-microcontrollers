#include <Arduino.h>
#include <unity.h>

#include "color_gradient_test.h"
#include "dip_switch_test.h"
#include "angle_test.h"


int main() {
    Serial.begin(115200);

    UNITY_BEGIN();

    angle_test();
    color_gradient_test();
    dip_switch_test();

    UNITY_END(); // stop unit testing
}

#include "roboarm.h"
#include <Arduino.h>

void roboarm_init(roboarm_t roboarm, uint8_t pin_J1, uint8_t pin_J2, uint8_t pin_clamp, uint8_t pin_sw)
{
    // initialize servos
    roboarm->servo_J1.attach(pin_J1);
    roboarm->servo_J1.write(ZERO_J1);
    roboarm->servo_J2.attach(pin_J2);
    roboarm->servo_J2.write(ZERO_J2);
    roboarm->servo_clamp.attach(pin_clamp);
    roboarm->servo_clamp.write(CLAMP_OPEN);

    // initialize switch
    pinMode(pin_sw, INPUT_PULLUP);
}

void roboarm_set_arm(roboarm_t roboarm, float height, float angle)
{
    double theta = degrees( asin((height - H_OFFSET) / ARM_LEN) );
    roboarm->servo_J1.write(ZERO_J1 + theta * DIR_J1);
    roboarm->servo_J2.write(ZERO_J2 + angle * DIR_J2);
}

void roboarm_arm_home(roboarm_t roboarm)
{
    roboarm->servo_J1.write(HOME_J1);
    roboarm->servo_J2.write(HOME_J2);
}

void roboarm_clamp_open(roboarm_t roboarm)
{
    roboarm->servo_clamp.write(CLAMP_OPEN);
}

void roboarm_clamp_close(roboarm_t roboarm)
{
    roboarm->servo_clamp.write(CLAMP_CLOSE);
}

bool roboarm_clamp_get_sw(roboarm_t roboarm)
{
    return digitalRead(roboarm->pin_sw);
}

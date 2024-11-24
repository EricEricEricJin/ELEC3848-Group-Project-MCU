#include "roboarm.h"
#include <Arduino.h>
#include "my_math.h"

void roboarm_init(roboarm_t roboarm, uint8_t pin_J1, uint8_t pin_J2, uint8_t pin_clamp, uint8_t pin_sw)
{
    // initialize servos
    roboarm->pin_J1 = pin_J1;
    roboarm->pin_J2 = pin_J2;
    roboarm->pin_clamp = pin_clamp;
    roboarm->pin_sw = pin_sw;

    roboarm->servo_J1.write(ZERO_J1);
    roboarm->servo_J2.write(ZERO_J2);
    roboarm->servo_clamp.write(CLAMP_OPEN);

    // initialize switch
    pinMode(pin_sw, INPUT_PULLUP);
}

void roboarm_enable(roboarm_t roboarm)
{
    if (!roboarm->enabled)
    {
        roboarm->servo_J1.attach(roboarm->pin_J1);
        roboarm->servo_J2.attach(roboarm->pin_J2);
        roboarm->servo_clamp.attach(roboarm->pin_clamp);
        roboarm->enabled = true;
    }
}

void roboarm_disable(roboarm_t roboarm)
{
    if (roboarm->enabled)
    {
        roboarm->servo_J1.detach();
        roboarm->servo_J2.detach();
        roboarm->servo_clamp.detach();
        roboarm->enabled = false;
    }
}



void roboarm_set_arm(roboarm_t roboarm, float height, float angle)
{
    float h = height - H_OFFSET;
    if (h > ARM_LEN)
        h = ARM_LEN;
    double theta = degrees( acos(h / (ARM_LEN + 0.1)) );

    // Serial.println(theta);
    roboarm_set_raw(roboarm, ZERO_J1 + theta * DIR_J1, ZERO_J2 + angle * DIR_J2);
}

void roboarm_set_raw(roboarm_t roboarm, int angle_J1, int angle_J2)
{
    VAL_LIMIT(angle_J1, J1_LIM_LO, J1_LIM_HI);
    VAL_LIMIT(angle_J2, J2_LIM_LO, J2_LIM_HI);

    roboarm->servo_J1.write(angle_J1);
    roboarm->servo_J2.write(angle_J2);
}


void roboarm_clamp_set_raw(roboarm_t roboarm, int angle)
{
    roboarm->servo_clamp.write(angle);
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

#include "chassis_task.h"

#include "motor.h"
#include "chassis.h"
#include "shared_mem.h"

#include "pid.h"

struct pid_param chassis_motor_param =
{
    .p = (float)MOTOR_DUTY_MAX / MAX_WHEEL_RPM,
    .i = 0.1f,
    .d = 0,
    .input_max_err = 0,
    .max_out = MOTOR_DUTY_MAX,
    .integral_limit = MOTOR_DUTY_MAX / 10.0f,
};

struct chassis chassis;

static float vx, vy, wz;

void chassis_setup()
{
    chassis_init(&chassis, chassis_motor_param);
}

void chassis_loop()
{
    // get speed
}
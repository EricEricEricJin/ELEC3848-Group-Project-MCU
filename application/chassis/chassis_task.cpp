#include "chassis_task.h"

#include "motor.h"
#include "chassis.h"
#include "shared_mem.h"
#include "msg_id.h"
#include "pid.h"
#include "chassis_cmd.h"

struct pid_param chassis_motor_param =
{
    .p = 0.8f,
    .i = 0.1f,
    .d = 0.0f,
    .input_max_err = 0,
    .max_out = MOTOR_DUTY_MAX,
    .integral_limit = MOTOR_DUTY_MAX / 5.0f,
};

struct chassis chassis;
struct chassis_cmd chassis_cmd;

void chassis_setup()
{
    chassis_init(&chassis, chassis_motor_param);
    chassis_set_offset(&chassis, ROTATE_X_OFFSET, ROTATE_Y_OFFSET);
    chassis_cmd.vx = 0;
    chassis_cmd.vy = 0;
    chassis_cmd.wz = 0;
    chassis_enable(&chassis);
}

void chassis_loop()
{
    // if (millis() > 5000)
    //     chassis_cmd.vx = 160;
    shared_mem_get(MSG_CHASSISCMD, &chassis_cmd);
    chassis_set_speed(&chassis, chassis_cmd.vx, chassis_cmd.vy, chassis_cmd.wz);
    chassis_calculate(&chassis);
}
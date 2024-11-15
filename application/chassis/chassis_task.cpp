#include "chassis_task.h"

#include "motor.h"
#include "chassis.h"
#include "shared_mem.h"
#include "msg_id.h"
#include "pid.h"
#include "chassis_cmd.h"

#include "follower.h"

const int LINE_FOLLOW_SPEED = 100;

struct pid_param chassis_motor_param =
{
    .p = 0.8f,
    .i = 0.1f,
    .d = 0.0f,
    .input_max_err = 0,
    .max_out = MOTOR_DUTY_MAX,
    .integral_limit = MOTOR_DUTY_MAX / 5.0f,
};

struct pid_param line_follow_param = 
{
    .p = 0.5f,
    .i = 0.05f,
    .d = 0.0f,
    .input_max_err = 0,
    .max_out = 2,
    .integral_limit = 0.5
};

struct chassis chassis;
struct chassis_cmd chassis_cmd;

struct follower follower;

void chassis_setup()
{
    // initialize chassis
    chassis_init(&chassis, chassis_motor_param);
    chassis_set_offset(&chassis, ROTATE_X_OFFSET, ROTATE_Y_OFFSET);
    chassis_cmd.vx = 0;
    chassis_cmd.vy = 0;
    chassis_cmd.wz = 0;
    chassis_enable(&chassis);

    // initialize line follower
    follower_init(&follower, line_follow_param);
    follower_set_head(&follower, 0);

}

void chassis_loop()
{
    shared_mem_get(MSG_CHASSISCMD, &chassis_cmd);
    
    float vx, vy, wz;
    float vleft, vright;

    //////////////////////////////
    // TEST LINE FOLLOW ONLY!   //
    chassis_cmd.op_mode = OP_FOLLOW;
    //////////////////////////////

    if (chassis_cmd.op_mode == OP_DIRECT)
    {
        chassis_set_mode(&chassis, CHASSIS_MODE_MECANUM);
        vx = chassis_cmd.vx;
        vy = chassis_cmd.vy;
        wz = chassis_cmd.wz;
        chassis_set_speed(&chassis, vx, vy, wz);   
    }
    else if (chassis_cmd.op_mode == OP_FOLLOW)
    {
        chassis_set_mode(&chassis, CHASSIS_MODE_TWOWHEEL);
        follower_calculate(&follower);
        vleft = LINE_FOLLOW_SPEED * follower_get_info(&follower)->left;
        vright = LINE_FOLLOW_SPEED * follower_get_info(&follower)->right;
        chassis_set_twowheel(&chassis, vleft, vright);
    }

    chassis_calculate(&chassis);
}
#include "chassis_task.h"

#include "motor.h"
#include "chassis.h"
#include "shared_mem.h"
#include "pid.h"
#include "chassis_protocol.h"
#include "app_cfg.h"

#include "communicate_task.h"

#include "follower.h"

#include "roboarm_protocol.h"

#include "sys.h"

#define CHASSIS_CMD_TIMEOUT_MS (500)

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

struct follower follower;

struct chassis_cmd chassis_cmd;
struct roboarm_cmd roboarm_cmd;
struct sensor_info sensor_info;

void forward_roboarm_cmd()
{
    communication_send(&com_S3, ROBOARM_CMD_ID, &roboarm_cmd, sizeof(roboarm_cmd));
}

void forward_sensor_info()
{
    communication_send(&com_S2, SENSOR_INFO_ID, &sensor_info, sizeof(sensor_info));
}

void chassis_setup()
{
    // initialize communication

    // route roboarm command
    communication_register_recv(&com_S2, ROBOARM_CMD_ID, &roboarm_cmd, sizeof(roboarm_cmd), (recv_callback_t)&forward_roboarm_cmd);
    // route sensor info
    communication_register_recv(&com_S3, SENSOR_INFO_ID, &sensor_info, sizeof(sensor_info), (recv_callback_t)&forward_sensor_info);
    // chassis cmd
    communication_register_recv(&com_S2, CHASSIS_CMD, &chassis_cmd, sizeof(chassis_cmd), NULL);

    // initialize chassis
    chassis_init(&chassis, chassis_motor_param);
    chassis_set_offset(&chassis, ROTATE_X_OFFSET, ROTATE_Y_OFFSET);
    chassis_cmd.vx = 0;
    chassis_cmd.vy = 0;
    chassis_cmd.wz = 0;
    // chassis_enable(&chassis);
    // chassis_disable(&chassis);

    // initialize line follower
    follower_init(&follower, line_follow_param);
    follower_set_head(&follower, 0);

}

void chassis_loop()
{    
    float vleft, vright;
    float vx, vy, wz;

    //////////////////////////////
    // TEST LINE FOLLOW ONLY!   //
    //////////////////////////////

    if (get_time_ms() - communication_get_recv_time_ms(&com_S2, CHASSIS_CMD) > CHASSIS_CMD_TIMEOUT_MS)
    {
        chassis_disable(&chassis);
    }
    else
    {
        chassis_enable(&chassis);
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
    }

    chassis_calculate(&chassis);

    // todo: send back data
}
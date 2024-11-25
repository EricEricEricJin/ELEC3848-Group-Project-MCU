#include "chassis_task.h"

#include "motor.h"
#include "chassis.h"
#include "shared_mem.h"
#include "pid.h"
#include "protocol.h"
#include "app_cfg.h"

#include "communicate_task.h"

#include "follower.h"


#include "sys.h"

#define CHASSIS_CMD_TIMEOUT_MS (1000)

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
struct chassis_fdbk chassis_fdbk;
struct roboarm_cmd fwd_roboarm_cmd;
struct sensor_fdbk fwd_sensor_fdbk;

struct chassis_info chassis_info;

void forward_roboarm_cmd()
{
    communication_send(&com_S3, ROBOARM_CMD_ID, &fwd_roboarm_cmd, sizeof(fwd_roboarm_cmd));
}

void forward_sensor_fdbk()
{
    communication_send(&com_S2, SENSOR_FDBK_ID, &fwd_sensor_fdbk, sizeof(fwd_sensor_fdbk));
}

void chassis_setup()
{
    // initialize communication

    // route roboarm command
    communication_register_recv(&com_S2, ROBOARM_CMD_ID, &fwd_roboarm_cmd, sizeof(fwd_roboarm_cmd), (recv_callback_t)&forward_roboarm_cmd);
    // route sensor info
    communication_register_recv(&com_S3, SENSOR_FDBK_ID, &fwd_sensor_fdbk, sizeof(fwd_sensor_fdbk), (recv_callback_t)&forward_sensor_fdbk);
    // chassis cmd
    communication_register_recv(&com_S2, CHASSIS_CMD_ID, &chassis_cmd, sizeof(chassis_cmd), NULL);

    // initialize chassis
    chassis_init(&chassis, chassis_motor_param);
    chassis_set_offset(&chassis, ROTATE_X_OFFSET, ROTATE_Y_OFFSET);
    // chassis_cmd.vx = 0;
    // chassis_cmd.vy = 0;
    // chassis_cmd.wz = 0;
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

    chassis_enable(&chassis);

    //////////////////////////////
    // TEST LINE FOLLOW ONLY!   //
    //////////////////////////////
    // chassis_set_mode(&chassis, CHASSIS_MODE_TWOWHEEL);

    // follower_calculate(&follower);
    // vleft = 100 * follower_get_info(&follower)->left;
    // vright = 100 * follower_get_info(&follower)->right;
    // chassis_set_twowheel(&chassis, vleft, vright);
    
    // Serial.print("VLEFT = ");
    // Serial.print(vleft);
    // Serial.print("VRGHT = ");
    // Serial.println(vright);

    // chassis_set_mode(&chassis, CHASSIS_MODE_MECANUM);
    // chassis_set_speed(&chassis, 100, 0, 0);

    


    if (get_time_ms() - communication_get_recv_time_ms(&com_S2, CHASSIS_CMD_ID) > CHASSIS_CMD_TIMEOUT_MS)
    {
        // Serial.println("Timeout!");
        chassis_disable(&chassis);
    }
    else
    {
        if (chassis_cmd.op_mode == CHASSIS_OP_DISABLE)
        {
            chassis_disable(&chassis);
        }
        if (chassis_cmd.op_mode == CHASSIS_OP_DIRECT)
        {
            chassis_enable(&chassis);
            chassis_set_mode(&chassis, CHASSIS_MODE_MECANUM);
            
            vx = chassis_cmd.vx;
            vy = chassis_cmd.vy;
            wz = chassis_cmd.wz;
            
            chassis_set_speed(&chassis, vx, vy, wz);   
        }
        else if (chassis_cmd.op_mode == CHASSIS_OP_FOLLOW)
        {
            chassis_enable(&chassis);
            chassis_set_mode(&chassis, CHASSIS_MODE_TWOWHEEL);
            
            follower_set_head(&follower, chassis_cmd.vx < 0);
            follower_calculate(&follower);
            vleft = chassis_cmd.vx * follower_get_info(&follower)->left;
            vright = chassis_cmd.vx * follower_get_info(&follower)->right;
            
            // Serial.print("VLEFT = ");
            // Serial.print(vleft);
            // Serial.print("VRGHT = ");
            // Serial.println(vright);

            chassis_set_twowheel(&chassis, vleft, vright);
        }
        else 
        {
            chassis_disable(&chassis);
            // chassis_set_mode(&chassis, CHASSIS_MODE_MECANUM);
            // chassis_set_speed(&chassis, 0, 0, 0);
        }
    }

    chassis_calculate(&chassis);
    chassis_get_info(&chassis, &chassis_info);


    // Send chassis feedback
    chassis_fdbk.line_sensor = follower_get_raw(&follower);
    chassis_fdbk.line_stop = follower_get_end(&follower);
    
    chassis_fdbk.mec_angle_deg = chassis_info.angle_deg;
    chassis_fdbk.mec_pos_x_mm = chassis_info.position_x_mm;
    chassis_fdbk.mec_pos_x_mm = chassis_info.position_y_mm;

    communication_send(&com_S2, CHASSIS_FDBK_ID, &chassis_fdbk, sizeof(chassis_fdbk));

    // todo: send back data
}
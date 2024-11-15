#ifndef __CHASSIS_H__
#define __CHASSIS_H__

#include "motor.h"
#include "pid.h"
#include "mecanum.h"

typedef struct chassis* chassis_t;
typedef struct chassis_info* chassis_info_t; 

typedef enum {
    CHASSIS_MODE_MECANUM,
    CHASSIS_MODE_TWOWHEEL,
} chassis_mode_t; 

// struct chassis_acc
// {
//     float ax;
//     float ay;
//     float wz;
// };

struct chassis
{
    struct mecanum mecanum;
    // struct chassis_acc acc;
    struct pid motor_pid[4];

    motor_device_t motor[4];

    chassis_mode_t mode;
    float vleft, vright;

};

struct chassis_info
{
    float v_x_mm;
    float v_y_mm;
    float rate_deg;
    float position_x_mm;
    float position_y_mm;
    float angle_deg;
    float yaw_gyro_angle;
    float yaw_gyro_rate;
    float wheel_rpm[4];
};

void chassis_init(chassis_t chassis, struct pid_param param);

void chassis_calculate(chassis_t chassis);

void chassis_set_mode(chassis_t chassis, chassis_mode_t mode);
void chassis_set_twowheel(chassis_t chassis, float vleft, float vright);

void chassis_gyro_update(chassis_t chassis, float yaw_angle, float yaw_rate);


void chassis_set_speed(chassis_t chassis, float vx, float vy, float vw);
// void chassis_set_acc(chassis_t chassis, float ax, float ay, float wz);

void chassis_set_offset(chassis_t chassis, float offset_x, float offset_y);

void chassis_get_info(chassis_t chassis, chassis_info_t info);

void chassis_enable(chassis_t chassis);
void chassis_disable(chassis_t chassis);

#endif 
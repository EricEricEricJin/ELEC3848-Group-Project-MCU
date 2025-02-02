#include "chassis.h"
#include "my_math.h"

MOTOR_ASSIGN(lft_frt, 34, 35, 12, 18, 31);
MOTOR_ASSIGN(rgt_frt, 37, 36, 8, 19, 38);
MOTOR_ASSIGN(lft_rr, 43, 42, 9, 3, 49);
MOTOR_ASSIGN(rgt_rr, PIN_A4, PIN_A5, 5, 2, PIN_A1);

motor_device_t motor_list[4] = {&rgt_frt, &lft_frt, &lft_rr, &rgt_rr};

void chassis_init(chassis_t chassis, struct pid_param param)
{

    chassis->mecanum.param.wheel_perimeter = PERIMETER;
    chassis->mecanum.param.wheeltrack = WHEELTRACK;
    chassis->mecanum.param.wheelbase = WHEELBASE;
    chassis->mecanum.param.rotate_x_offset = ROTATE_X_OFFSET;
    chassis->mecanum.param.rotate_y_offset = ROTATE_Y_OFFSET;

    for (int i = 0; i < 4; i++)
    {
        // register motor
        motor_register(motor_list[i]);
        chassis->motor[i] = motor_list[i];

        // initialize PID
        pid_struct_init(&chassis->motor_pid[i], param.max_out, param.integral_limit, param.p, param.i, param.d);
    }

}

void chassis_calculate(chassis_t chassis)
{
    // static uint8_t init_f = 0; // init flag
    // static unsigned long last_time, period;

    float motor_out;
    struct motor_data *pdata;
    struct mecanum_motor_fdb wheel_fbd[4];

    float wheel_rpm[4];

    // if (!init_f)
    // {
    //     // initial call
    //     period = 0;
    //     last_time = micros();
    //     init_f = 1;
    // }
    // else 
    // {
    //     period = micros() - last_time;
    //     last_time = micros();

    //     chassis->mecanum.speed.vx += chassis->acc.ax * period / 1'000'000.0f;
    //     chassis->mecanum.speed.vy += chassis->acc.ay * period / 1'000'000.0f;
    //     chassis->mecanum.speed.vw += chassis->acc.wz * period / 1'000'000.0f;
    // }

    if (chassis->mode == CHASSIS_MODE_MECANUM)
    {
        mecanum_calculate(&(chassis->mecanum));
        for (int i = 0; i < 4; i++)
            wheel_rpm[i] = chassis->mecanum.wheel_rpm[i];
    }
    else if (chassis->mode == CHASSIS_MODE_TWOWHEEL)
    {
        wheel_rpm[1] = chassis->vleft;
        wheel_rpm[2] = chassis->vleft;
        wheel_rpm[0] = - chassis->vright;
        wheel_rpm[3] = - chassis->vright;
    }

    // Send to motor
    for (int i = 0; i < 4; i++)
    {
        motor_update_data(chassis->motor[i]);
        
        pdata = motor_get_data(chassis->motor[i]);
        wheel_fbd[i].total_ecd = pdata->total_angle;
        wheel_fbd[i].speed_rpm = pdata->speed_rpm;
                
        // Serial.print("Wheel "); Serial.print(i);
        // Serial.print(" SpeedRPM = ");
        // Serial.print(wheel_fbd[i].speed_rpm);
        // Serial.print(", TargetRPM = ");
        // Serial.print(chassis->mecanum.wheel_rpm[i]);
        // Serial.print(", TotalECD = ");
        // Serial.println(wheel_fbd[i].total_ecd);


        motor_out = pid_calculate(&chassis->motor_pid[i], pdata->speed_rpm, wheel_rpm[i]);
        
        // Serial.print("SetDuty = ");
        // Serial.println((int16_t)motor_out);

        // Serial.print("WheelRPM = ");
        // Serial.println((int)chassis->mecanum.wheel_rpm[i]);

        motor_set_duty(chassis->motor[i], (int16_t)motor_out);
    }
    // Serial.println(wheel_fbd[0].total_ecd);

    mecanum_position_measure(&(chassis->mecanum), wheel_fbd);

}

void chassis_set_mode(chassis_t chassis, chassis_mode_t mode)
{
    chassis->mode = mode;
}

void chassis_set_twowheel(chassis_t chassis, float vleft, float vright)
{
    chassis->vleft = vleft;
    chassis->vright = vright;
}

void chassis_gyro_update(chassis_t chassis, float yaw_angle, float yaw_rate)
{
    chassis->mecanum.gyro.yaw_gyro_angle = yaw_angle;
    chassis->mecanum.gyro.yaw_gyro_rate = yaw_rate;
}

void chassis_set_speed(chassis_t chassis, float vx, float vy, float vw)
{
    chassis->mecanum.speed.vx = vx;
    chassis->mecanum.speed.vy = vy;
    chassis->mecanum.speed.vw = vw;
    
    // Serial.print("Target vx = "); Serial.println(chassis->mecanum.speed.vx);
    // Serial.print("Target vy = "); Serial.println(chassis->mecanum.speed.vy);
    // Serial.print("Target vw = "); Serial.println(chassis->mecanum.speed.vw);
}


// void chassis_set_acc(chassis_t chassis, float ax, float ay, float wz)
// {
//     chassis->acc.ax = ax;
//     chassis->acc.ay = ay;
//     chassis->acc.wz = wz;
// }

void chassis_set_offset(chassis_t chassis, float offset_x, float offset_y)
{
    chassis->mecanum.param.rotate_x_offset = offset_x;
    chassis->mecanum.param.rotate_y_offset = offset_y;
}

void chassis_get_info(chassis_t chassis, chassis_info_t info)
{
    memcpy(info, &(chassis->mecanum.position), sizeof(struct mecanum_position));
    ANGLE_LIMIT_180(info->angle_deg, chassis->mecanum.position.angle_deg);
    ANGLE_LIMIT_180(info->yaw_gyro_angle, chassis->mecanum.gyro.yaw_gyro_angle);
    info->yaw_gyro_rate = chassis->mecanum.gyro.yaw_gyro_rate;

    for (int i = 0; i < 4; i++)
    {
        info->wheel_rpm[i] = chassis->mecanum.wheel_rpm[i] * MOTOR_DECELE_RATIO;
    }
}


void chassis_enable(chassis_t chassis)
{
    for (int i = 0; i < 4; i++)
        chassis->motor_pid[i].enable = 1;
}

void chassis_disable(chassis_t chassis)
{
    for (int i = 0; i < 4; i++)
        chassis->motor_pid[i].enable = 0;
}



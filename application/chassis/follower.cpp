#include <Arduino.h>
#include "follower.h"

void follower_init(follower_t follower, struct pid_param param)
{
    follower->sensor_front.pin_0 = 41;  // PG1
    follower->sensor_front.pin_1 = 40;  // PG0
    follower->sensor_front.pin_2 = 47;  // PL2
    follower->sensor_front.pin_3 = 48;  // PL1
    line_sensor_register(&follower->sensor_front);
    
    follower->sensor_rear.pin_0 = 33;   // PC4
    follower->sensor_rear.pin_1 = 32;   // PC5
    follower->sensor_rear.pin_2 = 30;   // PC7
    follower->sensor_rear.pin_3 = 29;   // PA7
    line_sensor_register(&follower->sensor_rear);

    pid_struct_init(&follower->pid, param.max_out, param.integral_limit, param.p, param.i, param.d);
}

void follower_calculate(follower_t follower)
{
    line_sensor_t sensor;
    int error;
    float pid_out;

    if (follower->head)
        sensor = &follower->sensor_rear;
    else
        sensor = &follower->sensor_front;

    line_sensor_update(sensor);
    
    if (line_sensor_get_stop(sensor))
    {
        follower->info.left = 0;
        follower->info.right = 0;
        follower->info.reached_end = true;
    }
    else 
    {
        error = line_sensor_get_error(sensor);
        pid_out = pid_calculate(&follower->pid, error, 0);
        // Serial.print("PID out = ");
        // Serial.println(pid_out);
        follower->info.left = 1 - pid_out;
        follower->info.right = 1 + pid_out;
        follower->info.reached_end = false;
    }
    // follower->info.state_front_rear = (line_sensor_get_value(&follower->sensor_front) << 4) | line_sensor_get_value(&follower->sensor_rear);
    // Serial.print("Line error = ");
    // Serial.println(error);
    
    // Serial.print("Line value = ");
    // Serial.println(line_sensor_get_value(&follower->sensor_front));
     
}


follower_info_t follower_get_info(follower_t follower)
{
    return &follower->info;
}

bool follower_get_end(follower_t follower)
{
    line_sensor_t sensor;
    if (follower->head)
        sensor = &follower->sensor_rear;
    else
        sensor = &follower->sensor_front;
    return line_sensor_get_stop(sensor);
}

uint8_t follower_get_raw(follower_t follower)
{
    line_sensor_t sensor;
    if (follower->head)
        sensor = &follower->sensor_rear;
    else
        sensor = &follower->sensor_front;

    line_sensor_update(sensor);
    return line_sensor_get_value(sensor);
}

void follower_set_head(follower_t follower, bool head)
{
    follower->head = head;
}
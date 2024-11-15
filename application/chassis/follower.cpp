#include <Arduino.h>
#include "follower.h"

void follower_init(follower_t follower, struct pid_param param)
{
    follower->sensor_front.pin_0 = 41;  // PG1
    follower->sensor_front.pin_1 = 40;  // PG0
    follower->sensor_front.pin_2 = 47;  // PL2
    follower->sensor_front.pin_3 = 48;  // PL1
    line_sensor_register(&follower->sensor_front);
    
    // follower->sensor_rear.pin_0 = PG0;
    // follower->sensor_rear.pin_1 = PG1;
    // follower->sensor_rear.pin_2 = PC4;
    // follower->sensor_rear.pin_3 = PC5;
    // line_sensor_register(&follower->sensor_rear);

    pid_struct_init(&follower->pid, param.max_out, param.integral_limit, param.p, param.i, param.d);
}

void follower_calculate(follower_t follower)
{
    line_sensor_t sensor;
    int error;

    if (follower->head)
        sensor = &follower->sensor_rear;
    else
        sensor = &follower->sensor_front;

    line_sensor_update(sensor);
    
    error = line_sensor_get_error(sensor);
    if (line_sensor_get_stop(sensor))
    {
        follower->info.wz = 0;
    }
    else 
    {
        follower->info.wz = pid_calculate(&follower->pid, error, 0);
    }   
    Serial.print("Line error = ");
    Serial.println(error);
    
    // Serial.print("Line value = ");
    // Serial.println(line_sensor_get_value(&follower->sensor_front));
     
}

follower_info_t follower_get_info(follower_t follower)
{
    return &follower->info;
}

void follower_set_head(follower_t follower, bool head)
{
    follower->head = head;
}
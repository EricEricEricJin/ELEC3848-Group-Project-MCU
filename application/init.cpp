#include "init.h"

#include <HardwareSerial.h>

#include "chassis_task.h"
#include "sensor_task.h"



void hw_setup()
{
    Serial.begin(115200);
}

void sys_setup()
{
}

void task_setup()
{
    chassis_setup();
    sensor_setup();
}

void hw_loop()
{

}

void sys_loop()
{

}

void task_loop()
{
    chassis_loop();
    sensor_loop();
    // char prt_buf[32];
    // sprintf(prt_buf, "out task = %p", &(get_sensor_data()->));
    // Serial.println(prt_buf);
    Serial.print("Distance: ");
    Serial.println(get_sensor_data()->distance);
    Serial.print("angle x: ");
    Serial.println(get_sensor_data()->angleX);
    Serial.print("angle y: ");
    Serial.println(get_sensor_data()->angleY);
    Serial.print("angle z: ");
    Serial.println(get_sensor_data()->angleZ);
    Serial.print("Maximum Voltage: ");
    Serial.println(get_sensor_data()->max_voltage);
    Serial.print("Maximum Current: ");
    Serial.println(get_sensor_data()->max_current);
}

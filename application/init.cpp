#include "init.h"

#include <HardwareSerial.h>

#include "chassis_task.h"
#include "sensor_task.h"
#include "communicate_task.h"
#include <Arduino.h>


void hw_setup()
{
    Serial.begin(115200);
    Serial2.begin(115200);
    Serial3.begin(115200);
}

void sys_setup()
{    
}

void task_setup()
{
    communicate_setup();
    chassis_setup();
    // sensor_setup();
}

void hw_loop()
{

}

void sys_loop()
{

}

void task_loop()
{
    // Serial.println("Enter task_loop!");
    communicate_loop();
    // Serial.println("communicate_loop done!");
    chassis_loop();
    // sensor_loop();

    // Serial.print("Distance: ");
    // Serial.println(get_sensor_data()->distance);
    // Serial.print("angle x: ");
    // Serial.println(get_sensor_data()->angleX);
    // Serial.print("angle y: ");
    // Serial.println(get_sensor_data()->angleY);
    // Serial.print("angle z: ");
    // Serial.println(get_sensor_data()->angleZ);
    // Serial.print("Maximum Voltage: ");
    // Serial.println(get_sensor_data()->max_voltage);
    // Serial.print("Maximum Current: ");
    // Serial.println(get_sensor_data()->max_current);
    
    delay(20);
}

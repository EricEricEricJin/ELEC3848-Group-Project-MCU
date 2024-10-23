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
    sensor_loop(&sensorData);
    Serial.print("Distance: ");
    Serial.println(sensorData.distance);
    Serial.print("angle x: ");
    Serial.println(sensorData.angleX);
    Serial.print("angle y: ");
    Serial.println(sensorData.angleY);
    Serial.print("angle z: ");
    Serial.println(sensorData.angleZ);
    Serial.print("Maximum Voltage: ");
    Serial.println(sensorData.max_voltage);
    Serial.print("Maximum Current: ");
    Serial.println(sensorData.max_current);
}

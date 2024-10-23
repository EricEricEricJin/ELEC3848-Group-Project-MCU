#include "init.h"

#include <HardwareSerial.h>

#include "chassis_task.h"
#include "sensor_task.h"

SensorData sensorData;

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
    Serial.println("Distance: ",sensorData.distance);
    Serial.println("angle x: ",sensorData.angleX);
    Serial.println("angle y: ",sensorData.angleY);
    Serial.println("angle z: ",sensorData.angleZ);
    Serial.println("Maximum Voltage: ",sensorData.max_voltage);
    Serial.println("Maximum Current: ",sensorData.max_current);

}

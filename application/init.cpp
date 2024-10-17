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
}
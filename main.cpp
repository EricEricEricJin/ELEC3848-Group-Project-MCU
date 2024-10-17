#include <Arduino.h>

#include "init.h"  

void setup()
{
    hw_setup();
    sys_setup();
    task_setup();
}

void loop()
{
    hw_loop();
    sys_loop();
    task_loop();
}
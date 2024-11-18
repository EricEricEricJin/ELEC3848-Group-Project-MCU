#include "init.h"

#include <HardwareSerial.h>

#include "chassis_task.h"
#include "roboarm_task.h"

#include "communicate_task.h"
#include <Arduino.h>

#include "sys.h"

#define APP_SEL_PIN (25)    // PA3

enum {
    APP_SEL_CHASSIS = 0,
    APP_SEL_ROBOARM = 1,
};

static int app_select;

static sys_clock main_clock = {0};


void hw_setup()
{
    Serial.begin(115200);
    Serial2.begin(115200);
    Serial3.begin(115200);
    delay(100);
}

void sys_setup()
{
    pinMode(APP_SEL_PIN, INPUT_PULLUP);
    app_select = digitalRead(APP_SEL_PIN);    
}

void task_setup()
{
    communicate_setup();
    if (app_select == APP_SEL_CHASSIS)
    {
        Serial.println("==========================");
        Serial.println("|| APPLICATION: CHASSIS ||");
        Serial.println("==========================");
        Serial.println();
        chassis_setup();
    }
    else if (app_select == APP_SEL_ROBOARM)
    {
        Serial.println("==========================");
        Serial.println("|| APPLICATION: ROBOARM ||");
        Serial.println("==========================");
        Serial.println();
        roboarm_setup();
    }
}

void hw_loop()
{

}

void sys_loop()
{

}

void task_loop()
{
    communicate_loop();
    if (app_select == APP_SEL_CHASSIS)
    {
        chassis_loop();
        delay_period_us(&main_clock, 100'000);
    }
    else if (app_select == APP_SEL_ROBOARM)
    {
        roboarm_loop(); 
        delay_period_us(&main_clock, 100'000);
    }

    // Serial.println("Enter task_loop!");
    // Serial.println("communicate_loop done!");
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

    // Serial.print("CPU UTIL =");
    // Serial.println(main_clock.cpu_util);
}

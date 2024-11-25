#include <Arduino.h>
#include "line_sensor.h"
#include "sys.h"

void line_sensor_register(line_sensor_t sensor)
{
    device_assert(sensor != NULL);
    
    // register pins
    pinMode(sensor->pin_0, INPUT_PULLUP);
    pinMode(sensor->pin_1, INPUT_PULLUP);
    pinMode(sensor->pin_2, INPUT_PULLUP);
    pinMode(sensor->pin_3, INPUT_PULLUP);
    
    Serial.print("Registered ");
    // Serial.println(sensor->pin_0);

    sensor->value = 0;
}

void line_sensor_update(line_sensor_t sensor)
{
    // Serial.print("Line update! value =");
    sensor->value = (digitalRead(sensor->pin_3) << 3) |
                    (digitalRead(sensor->pin_2) << 2) |
                    (digitalRead(sensor->pin_1) << 1) |
                    (digitalRead(sensor->pin_0));
    // Serial.println(sensor->value);
}

uint8_t line_sensor_get_value(line_sensor_t sensor)
{
    return sensor->value;
}

int line_sensor_get_error(line_sensor_t sensor)
{
    int error;
    error = (((sensor->value & 0b1000) >> 3) - ((sensor->value & 0b0001))) * 3;
    if (error == 0)
       error = ((sensor->value & 0b0100) >> 2) - ((sensor->value & 0b0010) >> 1);
    return error;
}

bool line_sensor_get_stop(line_sensor_t sensor)
{
    return sensor->value == 0b1111;
}
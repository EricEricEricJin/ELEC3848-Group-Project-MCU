#ifndef __LINE_SENSOR_H__
#define __LINE_SENSOR_H__

#include <stdint.h>

typedef struct line_sensor *line_sensor_t;

struct line_sensor
{
    uint8_t pin_0, pin_1, pin_2, pin_3;
    uint8_t value;
};

void line_sensor_register(line_sensor_t sensor);
void line_sensor_update(line_sensor_t sensor);

uint8_t line_sensor_get_value(line_sensor_t sensor);

int line_sensor_get_error(line_sensor_t sensor);

bool line_sensor_get_stop(line_sensor_t sensor);

#endif 
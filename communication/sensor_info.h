#ifndef __SENSOR_INFO_H__
#define __SENSOR_INFO_H__

#include <stdint.h>

typedef struct sensor_info *sensor_info_t;

struct __attribute__((__packed__)) sensor_info
{
    int16_t distance_x10;
    int16_t angleX_x10;
    int16_t angleY_x10;
    int16_t angleZ_x10;
};

#endif

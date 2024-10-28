/******************************************************************************
 * Copyright (C) 2024 Eric Jin <https://github.com/ericericericjin>
 *
 * Everyone is permitted to copy and distribute verbatim or modified copies
 * of this program, and changing it is allowed as long as the name is changed.
 *****************************************************************************/

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

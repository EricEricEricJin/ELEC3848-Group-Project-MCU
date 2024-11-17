#ifndef __ROBOARM_PROTOCOL_H__
#define __ROBOARM_PROTOCOL_H__

#include <stdint.h>

typedef struct sensor_info *sensor_info_t;

#define ROBOARM_CMD_ID (0x3)
#define SENSOR_INFO_ID (0x3)

struct __attribute__((__packed__)) roboarm_cmd
{
    int16_t distance_x10;
    int16_t angleX_x10;
    int16_t angleY_x10;
    int16_t angleZ_x10;
};

struct __attribute__((__packed__)) sensor_info
{
    int16_t distance_x10;
    int16_t angleX_x10;
    int16_t angleY_x10;
    int16_t angleZ_x10;
};

#endif

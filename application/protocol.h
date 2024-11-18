#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include <time.h>
#include <stdint.h>


#define CHASSIS_CMD_ID (0x1)
#define CHASSIS_DATA_ID (0x2)

#define ROBOARM_CMD_ID (0x3)
#define SENSOR_INFO_ID (0x4)


typedef struct chassis_cmd *chassis_cmd_t;

enum {
    CHASSIS_OP_FOLLOW, CHASSIS_OP_DIRECT
};

struct __attribute__((__packed__)) chassis_cmd
{
    uint8_t op_mode;
    int16_t vx, vy, wz; // mm/s or deg/s
};

struct __attribute__((__packed__)) chassis_fdbk
{
    uint8_t op_mode;
    int16_t vx, vy, wz; // mm/s or deg/s
};


typedef struct sensor_info *sensor_info_t;
enum {
    ROBOARM_OP_HOME, ROBOARM_OP_HA
};

struct __attribute__((__packed__)) roboarm_cmd
{
    uint8_t op_mode;
    uint16_t height_mm;
    int8_t angle_deg;
};

struct __attribute__((__packed__)) sensor_info
{
    int16_t distance_x10;
    int16_t angleX_x10;
    int16_t angleY_x10;
    int16_t angleZ_x10;
};

#endif
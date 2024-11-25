#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include <time.h>
#include <stdint.h>


#define CHASSIS_CMD_ID (0x1)
#define CHASSIS_FDBK_ID (0x2)

#define ROBOARM_CMD_ID (0x3)
#define SENSOR_FDBK_ID (0x4)


typedef struct chassis_cmd *chassis_cmd_t;

enum {
    CHASSIS_OP_DISABLE = 0,
    CHASSIS_OP_DIRECT = 1,
    CHASSIS_OP_FOLLOW = 2
};

struct __attribute__((__packed__)) chassis_cmd
{
    uint8_t op_mode;
    int16_t vx, vy, wz; // mm/s or deg/s
};

struct __attribute__((__packed__)) chassis_fdbk
{
    uint8_t line_stop;
    uint8_t line_sensor;
    int16_t mec_pos_x_mm, mec_pos_y_mm;
    int16_t mec_angle_deg;
};

#define ROBOARM_GET_OP(x) ((x) & 0x7f)
#define ROBOARM_GET_CLOSE(x) ((x) >> 7)

typedef struct sensor_fdbk *sensor_fdbk_t;
enum {
    ROBOARM_OP_DISABLE = 0,
    ROBOARM_OP_HOME = 1,
    ROBOARM_OP_HA = 2
};

struct __attribute__((__packed__)) roboarm_cmd
{
    uint8_t op_mode;
    uint16_t height_mm;
    int8_t angle_deg;
};

struct __attribute__((__packed__)) sensor_fdbk
{
    uint8_t is_holding;
    uint16_t distance_mm;
    
    int16_t angleX_x10;
    int16_t angleY_x10;
    int16_t angleZ_x10;

    uint16_t current_mA;
    uint16_t voltage_mV;
};

#endif
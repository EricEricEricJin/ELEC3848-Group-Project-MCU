#ifndef __CHASSIS_PROTOCOL_H__
#define __CHASSIS_PROTOCOL_H__

#include <time.h>
#include <stdint.h>

typedef struct chassis_cmd *chassis_cmd_t;

typedef enum {
    OP_FOLLOW, OP_DIRECT
} op_mode_t;

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



#endif
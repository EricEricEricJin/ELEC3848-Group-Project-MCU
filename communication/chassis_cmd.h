/******************************************************************************
 * Copyright (C) 2024 Eric Jin <https://github.com/ericericericjin>
 *
 * Everyone is permitted to copy and distribute verbatim or modified copies
 * of this program, and changing it is allowed as long as the name is changed.
 *****************************************************************************/

#ifndef __CHASSIS_CMD_H__
#define __CHASSIS_CMD_H__

#include <time.h>
#include <stdint.h>

typedef struct chassis_cmd *chassis_cmd_t;

struct __attribute__((__packed__)) chassis_cmd
{
    int16_t vx, vy, wz; // mm/s or deg/s
};

#endif
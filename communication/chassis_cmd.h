/******************************************************************************
 * Copyright (C) 2024 Eric Jin <https://github.com/ericericericjin>
 *
 * Everyone is permitted to copy and distribute verbatim or modified copies
 * of this program, and changing it is allowed as long as the name is changed.
 *****************************************************************************/

#ifndef __GROUND_CMD_H__
#define __GROUND_CMD_H__

#include <time.h>
#include <stdint.h>

typedef struct chassis_cmd *chassis_cmd_t;

#define CMD_MAX_DELAY_MS (500)

// enum
// {
//     OPMODE_MANUAL = 0,
//     OPMODE_STABILIZE = 1,
//     OPMODE_LOCK_ATT = 2,
// };

struct __attribute__((__packed__)) chassis_cmd
{
    int16_t vx, vy, wz; // mm/s or deg/s
};

#endif
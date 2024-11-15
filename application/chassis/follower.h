#ifndef __FOLLOWER_H__
#define __FOLLOWER_H__

#include "line_sensor.h"
#include "pid.h"

typedef struct follower* follower_t;
typedef struct follower_info* follower_info_t;

struct follower_info {
    // int vx, vy, wz;
    int wz;
    bool reached_end;
};

struct follower {
    struct line_sensor sensor_front, sensor_rear;
    struct pid pid;
    struct follower_info info;
    bool head;
};

void follower_init(follower_t follower, struct pid_param param);
void follower_calculate(follower_t follower);
follower_info_t follower_get_info(follower_t follower);

void follower_set_head(follower_t follower, bool head);

#endif 
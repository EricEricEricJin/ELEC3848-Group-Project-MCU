#ifndef __SYS_H__
#define __SYS_H__

#include "stdint.h"

#define device_assert(EX) if (!(EX)) { while(1); }

typedef struct sys_clock* sys_clock_t;
struct sys_clock
{
    float cpu_util;
    bool cpu_full;
    uint32_t last_us;
};


uint32_t get_time_ms(void);
uint32_t get_time_us(void);
float get_time_ms_us(void);

void delay_period_us(sys_clock_t clock, unsigned long period);

#endif 
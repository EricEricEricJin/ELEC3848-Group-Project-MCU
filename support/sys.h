#ifndef __SYS_H__
#define __SYS_H__

#include "stdint.h"

#define device_assert(EX) if (!(EX)) { while(1); }

uint32_t get_time_ms(void);
uint32_t get_time_us(void);
float get_time_ms_us(void);

#endif 
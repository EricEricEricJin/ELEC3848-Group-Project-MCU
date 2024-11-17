#include <Arduino.h>

#include "sys.h"

uint32_t get_time_ms(void)
{
    return millis();
}

uint32_t get_time_us(void)
{
    return micros();
}

float get_time_ms_us(void)
{
    return micros() / 1000.0f;
}

void delay_period_us(sys_clock_t clock, unsigned long period)
{
    long t = period + clock->last_us - micros();
    if (t > 0)
    {
        delayMicroseconds(t);
        clock->cpu_util = 1.0f - (float)t / (float)period;
        clock->cpu_full = 0;
    }
    else
    {
        clock->cpu_util = 1.0f;
        clock->cpu_full = 1;
    }
    clock->last_us = micros();
}
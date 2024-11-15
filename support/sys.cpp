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


/******************************************************************************
 * Copyright (C) 2024 Eric Jin <https://github.com/ericericericjin>
 *
 * Everyone is permitted to copy and distribute verbatim or modified copies
 * of this program, and changing it is allowed as long as the name is changed.
 *****************************************************************************/

#ifndef __CHASSIS_DATA_H__
#define __CHASSIS_DATA_H__

#include <stdint.h>

typedef struct chassis_info_t *chassis_info;

#define DATA_MAX_DELAY_MS (100)
struct __attribute__((__packed__)) chassis_info
{
    // Motion
    int16_t gyro_yaw;       // YAW angle, degree x10
    int16_t vx, vy, wz;     // speeds, mm/s

    // Ultrasonic
    uint16_t tof;           // TOF or ultrasonic, mm

    // Battery voltage
    uint8_t volt_bat;

    // INA226    
    uint8_t wireless_volt, wireless_current;

    // BMP280
    int16_t pressure;
    int16_t temperature;

    // update time and checksum
    uint32_t update_time_ms;
    uint16_t crc16;
};

struct fdbk_data 
{
    int8_t elevator, aileron_l, aileron_r, rudder, eng_1, eng_2;
};

#endif

#ifndef __SENSORS_H__
#define __SENSORS_H__

#include "Adafruit_VL53L0X.h"
#include <Arduino.h>
#include <INA226.h>
#include <MPU6050_light.h>

typedef struct sensor_info* sensor_info_t;
typedef struct sensors* sensors_t;

struct sensor_info {
    float mpu_angleX, mpu_angleY, mpu_angleZ;
    float ina_volt, ina_amp;
    uint16_t tof_mm;
    bool sw;
};


struct sensors
{
    uint8_t sw_pin;
    Adafruit_VL53L0X* tof;
    INA226* ina;
    MPU6050* mpu;
    sensor_info info;
};


// SensorData_t get_sensor_data();

uint8_t sensor_setup(sensors_t sensors);

void sensor_update(sensors_t sensors);

sensor_info_t sensor_get_info(sensors_t sensors);

#endif
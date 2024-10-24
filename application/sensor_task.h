#ifndef __sensor_TASK_H__
#define __sensor_TASK_H__


struct SensorData {
    long distance;
    float angleX;
    float angleY;
    float angleZ;
    long max_voltage;
    long max_current;
};

typedef struct SensorData* SensorData_t;


void ultra_setup();
void gyro_setup();
void INA226_setup();
void measure_distance();
void measure_angle();
void INA226_measure();

SensorData_t get_sensor_data();

void sensor_setup();
void sensor_loop();  

#endif
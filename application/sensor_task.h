#ifndef __sensor_TASK_H__
#define __sensor_TASK_H__

typedef struct SensorData {
    long distance;
    float angleX;
    float angleY;
    float angleZ;
    long max_voltage;
    long max_current;
} SensorData;

void sensor_setup();
void sensor_loop(SensorData* sensorData);  
#endif
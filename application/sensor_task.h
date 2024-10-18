#ifndef __sensor_TASK_H__
#define __sensor_TASK_H__

typedef struct SensorData *Sensordata;

// Struct to store sensor data
struct SensorData {
    long distance;
    float angleX;
    float angleY;
    float angleZ;
};

void sensor_setup();
void sensor_loop(SensorData sensorData);
#endif
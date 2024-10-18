#ifndef __sensor_TASK_H__
#define __sensor_TASK_H__

typedef struct SensorData *Sensordata;
<<<<<<< HEAD
=======

void sensor_setup();
void sensor_loop(SensorData sensorData);

>>>>>>> 4b808cdccb88bd104c3ecd5de7f8a704fac8a095

// Struct to store sensor data
struct SensorData {
    long distance;
    float angleX;
    float angleY;
    float angleZ;
};
<<<<<<< HEAD

void sensor_setup();
void sensor_loop(SensorData sensorData);
#endif
=======
#endif
>>>>>>> 4b808cdccb88bd104c3ecd5de7f8a704fac8a095

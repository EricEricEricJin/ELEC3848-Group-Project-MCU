#ifndef __SENSOR_TASK_H__
#define __SENSOR_TASK_H__


struct SensorData {
    float distance;
    float angleX;
    float angleY;
    float angleZ;
    float max_voltage;
    float max_current;
};

typedef struct SensorData* SensorData_t;


// SensorData_t get_sensor_data();

void sensor_setup();
void sensor_loop();  

#endif
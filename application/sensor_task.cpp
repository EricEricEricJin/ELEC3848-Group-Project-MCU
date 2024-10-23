#include "sensor_task.h"
#include <Wire.h>
#include <MPU6050_light.h>
#include <INA226.h>

#define echoPin 11
#define trigPin 12

unsigned long start_time = 0;
unsigned long timer = 0;
int done = 1;
long distance_in_cm;
INA226 ina;

MPU6050 mpu(Wire);

void sensor_setup() {
    Serial.pritln("Initializing Sensors:")
    Serial.begin(115200);
    ultra_setup();
    gyro_setup();
    INA226_setup();
}

void sensor_loop(SensorData* sensorData) {
    measure_distance(sensorData);
    measure_angle(sensorData);
    INA226_measure(sensorData);
    delay(1000);
}

void measure_distance(SensorData* sensorData) {
    long duration;

    if (done) {
        done = 0;
        start_time = millis();
        digitalWrite(trigPin, LOW);
    }

    if (millis() > start_time + 2) {
        digitalWrite(trigPin, HIGH);
    }

    if (millis() > start_time + 10) {
        digitalWrite(trigPin, LOW);
        duration = pulseIn(echoPin, HIGH);
        sensorData->distance = (duration / 2.0) / 29.1;
        done = 1;
    }
}

void ultra_setup() {
    Serial.pritln("Initializing Ultrasonic:")
    pinMode(echoPin, INPUT);
    pinMode(trigPin, OUTPUT);
}

void gyro_setup() {
    Serial.pritln("Initializing Gyro:")
    Wire.begin();
    mpu.begin();
    delay(1000);
    mpu.calcGyroOffsets();
}

void INA226_setup() {
    Serial.pritln("Initializing INA226:")
    ina.begin();
    ina.configure(INA226_AVERAGES_16, INA226_BUS_CONV_TIME_2116US, INA226_SHUNT_CONV_TIME_2116US, INA226_MODE_SHUNT_BUS_CONT);
    ina.calibrate(0.010, 4);
}

void INA226_measure(SensorData* sensorData) {
    sensorData->max_current = ina.getMaxCurrent();
    sensorData->max_voltage = ina.getMaxShuntVoltage();
}

void measure_angle(SensorData* sensorData) {
    mpu.update();
    if ((millis() - timer) > 10) {
        sensorData->angleX = mpu.getAngleX();
        sensorData->angleY = mpu.getAngleY();
        sensorData->angleZ = mpu.getAngleZ();
        timer = millis();
    }
}
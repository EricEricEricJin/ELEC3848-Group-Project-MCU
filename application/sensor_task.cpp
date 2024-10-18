#include "sensor_task.h"
#include <Wire.h>
#include <MPU6050_light.h>

#define echoPin 11
#define trigPin 12

unsigned long start_time = 0;
unsigned long timer = 0;
int done = 1;
long distance_in_cm;

MPU6050 mpu(Wire); // Create MPU6050 object
void sensor_setup() {
    Serial.begin(115200);
    ultra_setup();
    gyro_setup();
}

void sensor_loop(SensorData sensorData) {
    measure_distance();
    measure_angle();
    Serial.println("Distance: ",sensorData.distance);
    Serial.println("angle x: ",sensorData.angleX);
    Serial.println("angle y: ",sensorData.angleY);
    Serial.println("angle z: ",sensorData.angleZ);
    delay(1000); // Run loop every second
}

void measure_distance(SensorData sensorData) {
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
        sensorData.distance = (duration / 2.0) / 29.1; // Store distance
        done = 1;
    }
}

void ultra_setup() {
    pinMode(echoPin, INPUT);
    pinMode(trigPin, OUTPUT);
}

void gyro_setup() {
    Wire.begin();
    mpu.begin();
    Serial.println("Calculating gyro offsets, do not move MPU6050");
    delay(1000);
    mpu.calcGyroOffsets();
    Serial.println("Done");
}

void measure_angle(Sensordata sensorData) {
    mpu.update();
    if ((millis() - timer) > 10) {
        sensorData.angleX = mpu.getAngleX();
        sensorData.angleY = mpu.getAngleY();
        sensorData.angleZ = mpu.getAngleZ();
        timer = millis();
    }
}
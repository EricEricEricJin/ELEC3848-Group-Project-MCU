#include "sensor_task.h"
#include <Wire.h>
#include <MPU6050_light.h>
#include <INA226.h>
#include <HardwareSerial.h>
#define echoPin 22
#define trigPin 24

static struct SensorData sensorData;

unsigned long start_time = 0;
unsigned long timer = 0;
int done = 1;
long distance_in_cm;

INA226 INA0(0x40);
INA226 INA1(0x41);

MPU6050 mpu(Wire);

void sensor_setup() {
    Serial.println("Initializing Sensors:");
    ultra_setup();
    gyro_setup();
    INA226_setup();
}

void sensor_loop() {
    measure_distance();
    measure_angle();
    INA226_measure();
}

void measure_distance() {
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
        distance_in_cm=(duration / 2.0) / 29.1;
        sensorData.distance = (duration / 2.0) / 29.1;
        char prt_buf[32];
        sprintf(prt_buf, "in task = %p", &(sensorData.distance));
        Serial.println(prt_buf);
        done = 1;
    }
}

void ultra_setup() {
    Serial.println("Initializing Ultrasonic:");
    pinMode(echoPin, INPUT);
    pinMode(trigPin, OUTPUT);
}

void gyro_setup() {
    Serial.println("Initializing Gyro:");
    Wire.begin();
    mpu.begin();
    delay(1000);
    mpu.calcGyroOffsets();
}

SensorData_t get_sensor_data()
{
    return &sensorData;
}

void INA226_setup() {
    Serial.println("Initializing INA226:");
    INA0.begin();
    //ina.configure(INA226_AVERAGES_16, INA226_BUS_CONV_TIME_2116US, INA226_SHUNT_CONV_TIME_2116US, INA226_MODE_SHUNT_BUS_CONT);
    //ina.calibrate(0.010, 4);
}

void INA226_measure() {
    sensorData.max_current = INA0.getMaxCurrent();
    sensorData.max_voltage = INA0.getShuntVoltage();
}

void measure_angle() {
    mpu.update();
    if ((millis() - timer) > 10) {
        sensorData.angleX = mpu.getAngleX();
        sensorData.angleY = mpu.getAngleY();
        sensorData.angleZ = mpu.getAngleZ();
        timer = millis();
        Serial.println(mpu.getAngleX());
    }
}
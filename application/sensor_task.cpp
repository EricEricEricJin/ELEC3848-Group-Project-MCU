#include "sensor_task.h"
#include <Wire.h>
#include <MPU6050_light.h>
#include <INA226.h>
#include <HardwareSerial.h>

#include <Adafruit_SSD1306.h>

#include "shared_mem.h"
#include "msg_id.h"

#define echoPin 22  // PA0
#define trigPin 24  // PA2

#define SCREEN_ADDRESS (0x3C)

static struct SensorData sensorData;

unsigned long start_time = 0;
unsigned long timer = 0;
int done = 1;
long distance_in_cm;


MPU6050 mpu(Wire);

INA226 ina;


Adafruit_SSD1306 display(128, 32, &Wire, -1);

void ultra_setup();
void gyro_setup();
void INA226_setup();
void display_setup();

void measure_distance();
void measure_angle();
void INA226_measure();
void display_data();

void sensor_setup() {
    Serial.println("Initializing Sensors:");
    ultra_setup();
    // gyro_setup();
    INA226_setup();
    display_setup();


}

void sensor_loop() {
    measure_distance();
    // measure_angle();
    INA226_measure();
    display_data();

    shared_mem_update(MSG_SENSORDATA, &sensorData);
}


// SensorData_t get_sensor_data()
// {
//     return &sensorData;
// }





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
void INA226_setup() {
    Serial.println("Initializing INA226:");
    ina.begin();
    ina.configure(INA226_AVERAGES_16, INA226_BUS_CONV_TIME_2116US, INA226_SHUNT_CONV_TIME_2116US, INA226_MODE_SHUNT_BUS_CONT);
    ina.calibrate(0.010, 4);
    //ina.configure(INA226_AVERAGES_16, INA226_BUS_CONV_TIME_2116US, INA226_SHUNT_CONV_TIME_2116US, INA226_MODE_SHUNT_BUS_CONT);
    //ina.calibrate(0.010, 4);
}

void display_setup()
{
    display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
    display.display();
    delay(1000);
    display.clearDisplay();
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
        
        done = 1;
    }
}

void INA226_measure() {
    sensorData.max_voltage = ina.readBusVoltage();
    sensorData.max_current = ina.readShuntCurrent();
    Serial.println(sensorData.max_voltage);
    Serial.println(sensorData.max_current);
    Serial.println(); 
    Serial.println(); 
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

void display_data()
{
    display.clearDisplay();

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);

    display.print("D = ");
    display.print(sensorData.distance);
    display.println(" cm");

    display.print("V = ");
    display.print(sensorData.max_voltage);
    display.println(" v");

    display.print("I = ");
    display.print(sensorData.max_current);
    display.println(" A");

    display.display();
}
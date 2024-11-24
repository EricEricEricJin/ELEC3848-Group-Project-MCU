#include "sensors.h"
#include <Wire.h>


static inline uint8_t mpu_setup(MPU6050* mpu)
{
    if (mpu->begin() != 0)
        return 1;
    // Serial.println("Begun");
    delay(1000);
    mpu->calcGyroOffsets();
    return 0;
}

static inline uint8_t ina_setup(INA226* ina, float r_ohm, float i_max) 
{
    // Serial.println("Initializing INA226:");
    ina->begin();
    ina->configure(INA226_AVERAGES_16, INA226_BUS_CONV_TIME_2116US, INA226_SHUNT_CONV_TIME_2116US, INA226_MODE_SHUNT_BUS_CONT);
    ina->calibrate(r_ohm, i_max);
    
    return 0;
}

static inline uint8_t tof_setup(Adafruit_VL53L0X* tof)
{
    uint8_t ret = 0;
    if (tof->begin())
        ret |= 0;
    else
        ret |= 1;
 
    if (tof->startRangeContinuous())
        ret |= 0;
    else
        ret |= 1;
    
    return ret;
}

static inline void ina_measure(sensor_info_t info, INA226* ina)
{
    info->ina_volt = ina->readBusVoltage();
    info->ina_amp = ina->readShuntCurrent();
}

static inline void mpu_measure(sensor_info_t info, MPU6050* mpu)
{
    mpu->update();
    info->mpu_angleX = mpu->getAngleX();
    info->mpu_angleY = mpu->getAngleY();
    info->mpu_angleZ = mpu->getAngleZ();
}

static inline void tof_measure(sensor_info_t info, Adafruit_VL53L0X* tof)
{
    if (tof->isRangeComplete())
    {
        info->tof_mm = tof->readRange();
        // Serial.print("TOF = ");
        // Serial.println(info->tof_mm);
    }


}

// void display_data()
// {
//     display.clearDisplay();

//     display.setTextSize(1);
//     display.setTextColor(WHITE);
//     display.setCursor(0, 0);

//     display.print("D = ");
//     display.print(sensorData.distance);
//     display.println(" cm");

//     display.print("V = ");
//     display.print(sensorData.max_voltage);
//     display.println(" v");

//     display.print("I = ");
//     display.print(sensorData.max_current);
//     display.println(" A");

//     display.display();
// }

uint8_t sensor_setup(sensors_t sensors) {
    sensors->sw_pin = 25;    // PA3

    sensors->tof = new Adafruit_VL53L0X();
    sensors->ina = new INA226();
    sensors->mpu = new MPU6050(Wire);
    
    uint8_t status = 0;
    Serial.println("Initializing Sensors:");

    pinMode(sensors->sw_pin, INPUT_PULLUP);
    
    // Serial.print("Sensors initialize status = ");
    // Serial.println(status);

    status |= mpu_setup(sensors->mpu);

    Serial.print("Sensors initialize status = ");
    Serial.println(status);

    status |= ina_setup(sensors->ina, 0.01, 4);

    Serial.print("Sensors initialize status = ");
    Serial.println(status);

    status |= tof_setup(sensors->tof);

    Serial.print("Sensors initialize status = ");
    Serial.println(status);

    return status;
}

void sensor_update(sensors_t sensors) 
{
    mpu_measure(&sensors->info, sensors->mpu);
    ina_measure(&sensors->info, sensors->ina);
    tof_measure(&sensors->info, sensors->tof);
    sensors->info.sw = digitalRead(sensors->sw_pin);
    // Serial.print("TOF = ");
    // Serial.println(sensors->info.tof_mm);
}

sensor_info_t sensor_get_info(sensors_t sensors)
{
    return &sensors->info;
}
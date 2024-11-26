#include <Arduino.h>
#include "roboarm_task.h"
#include "roboarm.h"

#include "communicate_task.h"
#include "protocol.h"

#include <sensors.h>

#include "sys.h"
#include "my_math.h"

#define PIN_JOINT1 (6)  // PH3
#define PIN_JOINT2 (7)  // PH4
#define PIN_CLAMP (4)  // PG5
#define PIN_SWITCH (48)  // PL1

#define INDICATOR_PIN (28)  // PA6

#define ROBOARM_CMD_TIMEOUT_MS (1000)

struct roboarm roboarm;
struct roboarm_cmd roboarm_cmd;

struct sensor_fdbk sensor_fdbk;

struct sensors sensors;

void roboarm_setup()
{
    pinMode(INDICATOR_PIN, OUTPUT);
    digitalWrite(INDICATOR_PIN, 0);

    // initialize communication
    communication_register_recv(&com_S2, ROBOARM_CMD_ID, &roboarm_cmd, sizeof(roboarm_cmd), NULL);
    roboarm_init(&roboarm, PIN_JOINT1, PIN_JOINT2, PIN_CLAMP, PIN_SWITCH);
    
    if (sensor_setup(&sensors) == 0)
        digitalWrite(INDICATOR_PIN, 1);
}


void roboarm_loop()
{

    uint8_t op_mode = ROBOARM_GET_OP(roboarm_cmd.op_mode);
    uint8_t close = ROBOARM_GET_CLOSE(roboarm_cmd.op_mode);

    // Serial.print("op_mode = ");
    // Serial.print(op_mode);
    // Serial.print(", close = ");
    // Serial.println(close);

    if (get_time_ms() - communication_get_recv_time_ms(&com_S2, ROBOARM_CMD_ID) > ROBOARM_CMD_TIMEOUT_MS)
    {
        Serial.println("Timeout!");
        roboarm_disable(&roboarm);
    }
    else if (op_mode == ROBOARM_OP_DISABLE)
    {
        roboarm_disable(&roboarm);
    }
    else if (op_mode == ROBOARM_OP_HOME)
    {
        roboarm_enable(&roboarm);
        roboarm_arm_home(&roboarm);
        roboarm_clamp_open(&roboarm);
    }
    else if (op_mode == ROBOARM_OP_HA)
    {
        roboarm_enable(&roboarm);
        roboarm_set_arm(&roboarm, roboarm_cmd.height_mm, roboarm_cmd.angle_deg);
        
        if (close)
        {
            roboarm_clamp_close(&roboarm);
            // Serial.println("Close");
        }
        else 
        {
            roboarm_clamp_open(&roboarm);
        }
    }
    else 
    {
        roboarm_disable(&roboarm);
    }

    // Serial.print("ROBOARM OP_MODE = ");
    // Serial.print(roboarm_cmd.op_mode);

    // Serial.print(", HEIGHT = ");
    // Serial.print(roboarm_cmd.height_mm);

    // Serial.print(", ANGLE = ");
    // Serial.println(roboarm_cmd.angle_deg);
    // roboarm_set_raw(&roboarm, 90, 90);
    
    // roboarm_set_arm(&roboarm, 140, 0);

    // update sensors
    // Serial.println("Update sensor...");
    sensor_update(&sensors);
    // Serial.println("Sensor updated...");

    sensor_info_t info = sensor_get_info(&sensors);
    


    sensor_fdbk.angleX_x10 = info->mpu_angleX * 10;
    sensor_fdbk.angleY_x10 = info->mpu_angleY * 10;
    sensor_fdbk.angleZ_x10 = info->mpu_angleZ * 10;

    sensor_fdbk.voltage_mV = info->ina_volt * 1000;
    sensor_fdbk.current_mA = info->ina_amp * 1000;

    sensor_fdbk.distance_mm = info->tof_mm;
    sensor_fdbk.is_holding = roboarm_clamp_get_sw(&roboarm);
    
    // Serial.print("LOOP ");
    communication_send(&com_S2, SENSOR_FDBK_ID, &sensor_fdbk, sizeof(sensor_fdbk));

}
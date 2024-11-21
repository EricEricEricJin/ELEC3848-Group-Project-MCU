#include <Arduino.h>
#include "roboarm_task.h"
#include "roboarm.h"

#include "communicate_task.h"
#include "protocol.h"

#include <sensors.h>

#define PIN_JOINT1 (6)  // PH3
#define PIN_JOINT2 (7)  // PH4
#define PIN_CLAMP (4)  // PG5
#define PIN_SWITCH (48)  // PL1

struct roboarm roboarm;
struct roboarm_cmd roboarm_cmd;

struct sensor_fdbk sensor_fdbk;

struct sensors sensors;

void roboarm_setup()
{
    // initialize communication
    communication_register_recv(&com_S2, ROBOARM_CMD_ID, &roboarm_cmd, sizeof(roboarm_cmd), NULL);
    roboarm_init(&roboarm, PIN_JOINT1, PIN_JOINT2, PIN_CLAMP, PIN_SWITCH);
    
    sensor_setup(&sensors);

}

void roboarm_loop()
{

    if (roboarm_cmd.op_mode == ROBOARM_OP_HOME)
    {
        roboarm_arm_home(&roboarm);
    }
    else if (roboarm_cmd.op_mode == ROBOARM_OP_HA)
    {
        roboarm_set_arm(&roboarm, roboarm_cmd.height_mm, roboarm_cmd.angle_deg);
    }

    Serial.print("ROBOARM OP_MODE = ");
    Serial.print(roboarm_cmd.op_mode);

    Serial.print(", HEIGHT = ");
    Serial.print(roboarm_cmd.height_mm);

    Serial.print(", ANGLE = ");
    Serial.println(roboarm_cmd.angle_deg);
    // roboarm_set_raw(&roboarm, 115, 75);

    // update sensors
    sensor_update(&sensors);
    sensor_info_t info = sensor_get_info(&sensors);
    
    sensor_fdbk.angleX_x10 = info->mpu_angleX * 10;
    sensor_fdbk.angleY_x10 = info->mpu_angleY * 10;
    sensor_fdbk.angleZ_x10 = info->mpu_angleZ * 10;

    sensor_fdbk.voltage_mV = info->ina_volt * 1000;
    sensor_fdbk.current_mA = info->ina_amp * 1000;

    sensor_fdbk.distance_mm = info->tof_mm;
    sensor_fdbk.is_holding = info->sw;

    communication_send(&com_S2, SENSOR_FDBK_ID, &sensor_fdbk, sizeof(sensor_fdbk));

}
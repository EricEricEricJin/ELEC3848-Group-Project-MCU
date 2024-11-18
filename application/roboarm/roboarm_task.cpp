#include <Arduino.h>
#include "roboarm_task.h"
#include "roboarm.h"

#include "communicate_task.h"
#include "protocol.h"

#define PIN_JOINT1 (6)  // PH3
#define PIN_JOINT2 (7)  // PH4
#define PIN_CLAMP (4)  // PG5
#define PIN_SWITCH (48)  // PL1

struct roboarm roboarm;
struct roboarm_cmd roboarm_cmd;

void roboarm_setup()
{
    // initialize communication
    communication_register_recv(&com_S2, ROBOARM_CMD_ID, &roboarm_cmd, sizeof(roboarm_cmd), NULL);

    roboarm_init(&roboarm, PIN_JOINT1, PIN_JOINT2, PIN_CLAMP, PIN_SWITCH);
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

}
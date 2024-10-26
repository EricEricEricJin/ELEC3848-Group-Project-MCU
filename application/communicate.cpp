#include <Arduino.h>
#include "communicate.h"
#include "communication.h"
#include "chassis_cmd.h"
#include "shared_mem.h"
#include "msg_id.h"

struct communication comm_UART3;
struct chassis_cmd communicate_chassis_cmd;

#define CHASSIS_CMD_ID (0x01)

void communicate_setup()
{
    communication_setup(&comm_UART3, &Serial3, 128);
    shared_mem_create(MSG_CHASSISCMD, sizeof(struct chassis_cmd));
    communication_register_recv_buf(&comm_UART3, CHASSIS_CMD_ID, sizeof(struct chassis_cmd));
}

void communicate_loop()
{
    communication_loop(&comm_UART3);
    
    communication_recv(&comm_UART3, CHASSIS_CMD_ID, &communicate_chassis_cmd);
    shared_mem_update(MSG_CHASSISCMD, &communicate_chassis_cmd);
}

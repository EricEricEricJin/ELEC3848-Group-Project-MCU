#include <Arduino.h>
#include "communicate.h"
#include "communication.h"


#include "chassis_cmd.h"
#include "sensor_info.h"

#include "shared_mem.h"
#include "msg_id.h"

#include "sensor_task.h"

// Data path:
// raw_buf --> recv_buf --> communicate_* --> shared_mem --> target
// todo: raw_buf --> communicate_* --> shared_mem --> target

struct communication comm_UART3;

struct chassis_cmd comm_chassis_cmd;
struct sensor_info comm_sensor_info;

struct SensorData sensor_data;

#define CHASSIS_CMD_ID (0x01)
#define SENSOR_DATA_ID (0x02)

void communicate_setup()
{
    communication_setup(&comm_UART3, &Serial3, SERIAL_RX_BUFFER_SIZE);

    shared_mem_create(MSG_CHASSISCMD, sizeof(struct chassis_cmd));
    
    shared_mem_create(MSG_SENSORDATA, sizeof(struct SensorData));

    communication_register_recv_buf(&comm_UART3, CHASSIS_CMD_ID, sizeof(comm_chassis_cmd));
    communication_register_send_buf(&comm_UART3, SENSOR_DATA_ID, sizeof(comm_sensor_info));
}

void communicate_loop()
{
    // uplink
    shared_mem_get(MSG_SENSORDATA, &sensor_data);
    comm_sensor_info.distance_x10 = sensor_data.distance * 10;
    comm_sensor_info.angleX_x10 = sensor_data.angleX * 10;
    comm_sensor_info.angleY_x10 = sensor_data.angleY * 10;
    comm_sensor_info.angleZ_x10 = sensor_data.angleZ * 10;
    communication_send(&comm_UART3, SENSOR_DATA_ID, &comm_sensor_info);

    // loop
    communication_loop(&comm_UART3);
    
    // downlink
    communication_recv(&comm_UART3, CHASSIS_CMD_ID, &comm_chassis_cmd);
    shared_mem_update(MSG_CHASSISCMD, &comm_chassis_cmd);
}

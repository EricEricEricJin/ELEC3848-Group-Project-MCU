#include <Arduino.h>
#include "communicate_task.h"
#include <HardwareSerial.h>

// Data path:
// raw_buf --> recv_buf --> communicate_* --> shared_mem --> target
// todo: raw_buf --> communicate_* --> shared_mem --> target

struct communication com_S1 = {0}, com_S2 = {0}, com_S3 = {0};

void communicate_setup()
{
    communication_setup(&com_S1, &Serial1, SERIAL_RX_BUFFER_SIZE);
    communication_setup(&com_S2, &Serial2, SERIAL_RX_BUFFER_SIZE);
    communication_setup(&com_S3, &Serial3, SERIAL_RX_BUFFER_SIZE);
}

void communicate_loop()
{
    communication_loop(&com_S1);
    communication_loop(&com_S2);
    communication_loop(&com_S3);
}

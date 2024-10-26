#include <Arduino.h>

#include "init.h"

void setup()
{
    hw_setup();
    sys_setup();
    task_setup();
}

void loop()
{
    hw_loop();
    sys_loop();
    task_loop();
}

// #include "communication.h"

// struct communication comm;

// int16_t rb[3];

// unsigned long t_send;

// void setup()
// {
//     Serial.begin(115200);
//     Serial3.begin(115200);

//     communication_setup(&comm, &Serial3, 1024);

//     // communication_register_send_buf(&comm, 11, sizeof(sb));
//     communication_register_recv_buf(&comm, 0x01, sizeof(rb));

//     t_send = 0;
// }

// void loop()
// {
//     communication_loop(&comm);

//     if (millis() - t_send > 1000)
//     {
//         t_send = millis();
//         communication_recv(&comm, 0x01, rb);
//         Serial.println("Data:");
//         Serial.println(rb[0]);
//         Serial.println(rb[1]);
//         Serial.println(rb[2]);
//     }

//     delay(100);
// }
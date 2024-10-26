#include <Arduino.h>

// #include "init.h"

// void setup()
// {
//     hw_setup();
//     sys_setup();
//     task_setup();
// }

// void loop()
// {
//     hw_loop();
//     sys_loop();
//     task_loop();
// }

#include "communication.h"

struct communication comm;

char sb[3];
char rb[3];

unsigned long t_send;

void setup()
{
    Serial.begin(115200);
    communication_setup(&comm, &Serial, 1024);
    communication_register_send_buf(&comm, 11, sizeof(sb));
    communication_register_recv_buf(&comm, 22, sizeof(rb));

    t_send = 0;
}

void loop()
{
    int recv_flag;
    communication_loop(&comm);

    if (millis() - t_send > 1000)
    {
        t_send = millis();
        communication_recv(&comm, 22, sb);
        // sb[0] = 0x01; sb[1] = 0x02; sb[2] = 0x03;
        communication_send(&comm, 11, sb);
    }

    delay(100);
}
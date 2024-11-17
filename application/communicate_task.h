#ifndef __COMMUNICATE_TASK_H__
#define __COMMUNICATE_TASK_H__

#include "communication.h"

extern struct communication com_S1, com_S2, com_S3;

void communicate_setup();
void communicate_loop();

#endif 
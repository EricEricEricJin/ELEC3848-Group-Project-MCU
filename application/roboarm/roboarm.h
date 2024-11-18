#ifndef __ROBOARM_H__
#define __ROBOARM_H__

#include <stdint.h>
#include <Servo.h>

#define ARM_LEN (70)
#define H_OFFSET (83)

#define ZERO_J1 (90)
#define ZERO_J2 (90)

#define HOME_J1 (60)
#define HOME_J2 (90)

#define DIR_J1 (1)
#define DIR_J2 (-1)

#define CLAMP_OPEN (180)
#define CLAMP_CLOSE (70)

typedef struct roboarm* roboarm_t;

struct roboarm
{    
    uint8_t pin_sw;
    Servo servo_J1, servo_J2, servo_clamp;
    int lim_J1_a, lim_J1_b, lim_J2_a, lim_J2_b;
};

void roboarm_init(roboarm_t roboarm, uint8_t pin_J1, uint8_t pin_J2, uint8_t pin_clamp, uint8_t pin_sw);
void roboarm_set_arm(roboarm_t roboarm, float height, float angle);
void roboarm_arm_home(roboarm_t roboarm);

void roboarm_clamp_open(roboarm_t roboarm);
void roboarm_clamp_close(roboarm_t roboarm);
bool roboarm_clamp_get_sw(roboarm_t roboarm);

#endif 

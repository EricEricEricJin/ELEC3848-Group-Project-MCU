#include "chassis_task.h"

#include "motor.h"

#include <Arduino.h>

void lft_frt_isr(void);
void rgt_frt_isr(void);
void lft_rr_isr(void);
void rgt_rr_isr(void);

struct motor_device lft_frt = {
    .pin_1 = 34,
    .pin_2 = 35,
    .pwm_pin = 12,
    .ecd_A_pin = 18,
    .ecd_B_pin = 31,
    .ecd_isr = &lft_frt_isr
};


struct motor_device rgt_frt = {
    .pin_1 = 37,
    .pin_2 = 36,
    .pwm_pin = 8,
    .ecd_A_pin = 19,
    .ecd_B_pin = 38,
    .ecd_isr = &rgt_frt_isr
};

struct motor_device lft_rr = {
    .pin_1 = 43,
    .pin_2 = 42,
    .pwm_pin = 9,
    .ecd_A_pin = 3,
    .ecd_B_pin = 49,
    .ecd_isr = lft_rr_isr
};

struct motor_device rgt_rr = {
    .pin_1 = PIN_A4,
    .pin_2 = PIN_A5,
    .pwm_pin = 5,
    .ecd_A_pin = 2,
    .ecd_B_pin = 23,
    .ecd_isr = rgt_rr_isr
};

void lft_frt_isr(void) { lft_frt.data.total_ecd++; };
void rgt_frt_isr(void) { rgt_frt.data.total_ecd++; };
void lft_rr_isr(void) { lft_rr.data.total_ecd++; };
void rgt_rr_isr(void) { rgt_rr.data.total_ecd++; };

void chassis_setup()
{
    Serial.println("CHASSIS SETUP!");

    // setup motors
    motor_register(&lft_frt);
    motor_register(&rgt_frt);
    motor_register(&lft_rr);
    motor_register(&rgt_rr);

    //motor_set_duty(&lft_frt, 100);
    //motor_set_duty(&rgt_frt, 100);
    //motor_set_duty(&lft_rr, 100);
    //motor_set_duty(&rgt_rr, 100);
}


void chassis_loop()
{
    delay(100);   
}
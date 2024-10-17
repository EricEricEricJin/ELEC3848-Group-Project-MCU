#ifndef __MOTOR_H__
#define __MOTOR_H__

#include <stdint.h>

typedef struct motor_data *motor_data_t;
typedef struct motor_device *motor_device_t;

#define ECD_TICKS (200)

struct motor_data
{
    int32_t total_ecd;      // total number of ticks
    int32_t offset_ecd;     // zero position
    
    int ecd;                // position

    int32_t round_cnt;      // total number of rounds
    int32_t total_angle;    // total number of degs

    int16_t speed_rpm;      // rpm speed
    
    unsigned long last_upd_micros; 
    int32_t last_upd_total_ecd;
};

struct motor_device
{
    uint8_t pin_1, pin_2, pwm_pin, ecd_A_pin, ecd_B_pin;
    void (*ecd_isr)();  // set to ecd++
    int16_t duty;
    struct motor_data data;
};

void motor_register(motor_device_t motor_dev);
void motor_set_duty(motor_device_t motor_dev, int16_t duty);
motor_data_t motor_get_data(motor_device_t motor_dev);

#endif 
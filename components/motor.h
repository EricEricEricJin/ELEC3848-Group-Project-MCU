#ifndef __MOTOR_H__
#define __MOTOR_H__

#include <stdint.h>
#include <Arduino.h>

typedef struct motor_data *motor_data_t;
typedef struct motor_device *motor_device_t;

// #define ECD_TICKS (90)
#define ECD_TICKS (200)
#define MOTOR_DUTY_MAX (255)
#define MOTOR_NAME_MAX_LEN (32) 

// struct motor_device md;
#define MOTOR_ASSIGN(NAME, PIN_1, PIN_2, PIN_PWM, PIN_A, PIN_B)   \
void isr_##NAME();                                                  \
struct motor_device NAME = {                                        \
    .pin_1 = PIN_1, .pin_2 = PIN_2, .pin_pwm = PIN_PWM,             \
    .pin_ecd_A = PIN_A, .pin_ecd_B = PIN_B, .ecd_isr = isr_##NAME   \
};                                                                  \
void isr_##NAME(void) {                                             \
    if(digitalRead(NAME.pin_ecd_B)) NAME.data.total_ecd++;          \
    else NAME.data.total_ecd--;                                     \
}

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
    uint8_t pin_1, pin_2, pin_pwm, pin_ecd_A, pin_ecd_B;
    void (*ecd_isr)();
    struct motor_data data;
};

void motor_register(motor_device_t motor_dev);
void motor_set_duty(motor_device_t motor_dev, int16_t duty);
void motor_update_data(motor_device_t motor_dev);
motor_data_t motor_get_data(motor_device_t motor_dev);

#endif 
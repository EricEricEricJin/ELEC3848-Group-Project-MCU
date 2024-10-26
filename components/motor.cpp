#include "motor.h"
#include <Arduino.h>

void motor_register(motor_device_t motor_dev)
{
    // PWM
    pinMode(motor_dev->pin_1, OUTPUT);
    pinMode(motor_dev->pin_2, OUTPUT);
    pinMode(motor_dev->pin_pwm, OUTPUT);

    // Encoder 
    pinMode(motor_dev->pin_ecd_A, INPUT_PULLUP);
    pinMode(motor_dev->pin_ecd_B, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(motor_dev->pin_ecd_A), motor_dev->ecd_isr, FALLING);
}

void motor_set_duty(motor_device_t motor_dev, int16_t duty)
{
    bool dir = duty > 0;
    duty = abs(duty);
    duty = duty > 255 ? 255 : duty;

    digitalWrite(motor_dev->pin_1, !dir);
    digitalWrite(motor_dev->pin_2, dir);
    analogWrite(motor_dev->pin_pwm, duty);
}

void motor_update_data(motor_device_t motor_dev)
{
    motor_dev->data.ecd = motor_dev->data.total_ecd % ECD_TICKS;
    motor_dev->data.round_cnt = motor_dev->data.total_ecd / ECD_TICKS;
    motor_dev->data.total_angle = motor_dev->data.total_ecd * 360 / ECD_TICKS;
    
    motor_dev->data.speed_rpm = ((long)(motor_dev->data.total_ecd - motor_dev->data.last_upd_total_ecd) * (60000000L / ECD_TICKS)) / (long)(micros() - motor_dev->data.last_upd_micros);
    motor_dev->data.last_upd_total_ecd = motor_dev->data.total_ecd;
    motor_dev->data.last_upd_micros = micros();
}

motor_data_t motor_get_data(motor_device_t motor_dev)
{
    return &motor_dev->data;
}
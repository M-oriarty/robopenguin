#ifndef _PWM_H
#define _PWM_H

#include "tim.h"
#include "stdio.h"

#define PWM1 1//TIM9_CH1,PWM1
#define PWM2 2//TIM9_CH2,PWM2
#define PWM3 3//TIM4_CH1,PWM3
#define PWM4 4//TIM4_CH2,PWM4
#define PWM5 5//TIM4_CH3,PWM5
#define PWM6 6//TIM4_CH4,PWM6
#define PWM7 7//TIM1_CH3,PWM7
#define PWM8 8//TIM1_CH4,PWM8
#define PWM9 9//TIM1_CH1,PWM9

//¶æ»úpwmÇø¼ä
#define SERVO_FLAP_MAX_VALUE       1944 //40¡ã ÐØ÷¢flap
#define SERVO_FLAP_MIN_VALUE       1056 //40¡ã ÐØ÷¢flap
#define SERVO_FEATHER_MAX_VALUE    2000 //45¡ã ÐØ÷¢feather
#define SERVO_FEATHER_MIN_VALUE   1000 //45¡ã ÐØ÷¢feather
#define SERVO_PITCH_MAX_VALUE      1833 //30¡ã ÐØ÷¢pitch
#define SERVO_PITCH_MIN_VALUE      1167 //30¡ã ÐØ÷¢pitch
#define SERVO_TAIL_MAX_VALUE       1833 //30¡ã Î²÷¢
#define SERVO_TAIL_MIN_VALUE       1167 //30¡ã Î²÷¢

//¶æ»úÖÐÎ»
#define SERVO1_MID_VALUE 1500
#define SERVO2_MID_VALUE 1500
#define SERVO3_MID_VALUE 1500
#define SERVO4_MID_VALUE 1500
#define SERVO5_MID_VALUE 1500
#define SERVO6_MID_VALUE 1500
#define SERVO7_MID_VALUE 1500
#define SERVO8_MID_VALUE 1500
#define SERVO9_MID_VALUE 1500

void servo_pwm_set_compare( uint16_t compare_value, uint8_t channel );
void servo_init ( void );

#endif

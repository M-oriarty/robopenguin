#include "pwm.h"

/********************************************************************************
*
* Function name ：servo_pwm_set_compare
* Description   ：舵机初始化
*
* Parameter     ：
*       @compare_value ：舵机PWM的大小
*     @channel  ：舵机通道，共四路
* Return        ：null
********************************************************************************/
void servo_pwm_set_compare( uint16_t compare_value, uint8_t channel )
{
    switch(channel)
    {
        case PWM1:
            TIM_SetCompare1(TIM9,compare_value);
            break;
        case PWM2:
            TIM_SetCompare2(TIM9, compare_value);
            break;
        case PWM3:
            if(compare_value<=SERVO_FLAP_MAX_VALUE && compare_value>=SERVO_FLAP_MIN_VALUE){
                TIM_SetCompare1(TIM4, compare_value);
            }
            break;
        case PWM4:
            if(compare_value<=SERVO_FLAP_MAX_VALUE && compare_value>=SERVO_FLAP_MIN_VALUE){
                TIM_SetCompare2(TIM4, compare_value);
            }
            break;
        case PWM5:
            if(compare_value<=SERVO_FEATHER_MAX_VALUE && compare_value>=SERVO_FEATHER_MIN_VALUE){
                TIM_SetCompare3(TIM4, compare_value);
            }
            break;
        case PWM6:
            if(compare_value<=SERVO_FEATHER_MAX_VALUE && compare_value>=SERVO_FEATHER_MIN_VALUE){
                TIM_SetCompare4(TIM4, compare_value);
            }
            break;
        case PWM7:
            if(compare_value<=SERVO_PITCH_MAX_VALUE && compare_value>=SERVO_PITCH_MIN_VALUE){
                TIM_SetCompare3(TIM1, compare_value);
            }
            break;
        case PWM8:
            if(compare_value<=SERVO_PITCH_MAX_VALUE && compare_value>=SERVO_PITCH_MIN_VALUE){
                TIM_SetCompare4(TIM1, compare_value);
            }
            break;
        case PWM9:
            if(compare_value<=SERVO_TAIL_MAX_VALUE && compare_value>=SERVO_TAIL_MIN_VALUE){
                TIM_SetCompare1(TIM1, compare_value);
            }
            break;
        default:
            break;
    }
}
/********************************************************************************
*
* Function name ：servo_init
* Description   ：舵机初始化
*
* Parameter     ：
*       @null
* Return        ：null
********************************************************************************/
void servo_init ( void )
{
//    HAL_TIM_Base_Start(&htim4);
//    HAL_TIM_Base_Start(&htim9);
//    HAL_TIM_Base_Start(&htim1);
//    HAL_TIM_PWM_Start(&htim9,TIM_CHANNEL_1);
//    HAL_TIM_PWM_Start(&htim9,TIM_CHANNEL_2);
//    HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);
//    HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);
//    HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);
//    HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);
//    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
//    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
//    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);

    servo_pwm_set_compare(SERVO1_MID_VALUE,PWM1);
    servo_pwm_set_compare(SERVO2_MID_VALUE,PWM2);
    servo_pwm_set_compare(SERVO3_MID_VALUE,PWM3);
    servo_pwm_set_compare(SERVO4_MID_VALUE,PWM4);
    servo_pwm_set_compare(SERVO5_MID_VALUE,PWM5);
    servo_pwm_set_compare(SERVO6_MID_VALUE,PWM6);
    servo_pwm_set_compare(SERVO7_MID_VALUE,PWM7);
    servo_pwm_set_compare(SERVO8_MID_VALUE,PWM8);
    servo_pwm_set_compare(SERVO9_MID_VALUE,PWM9);
}

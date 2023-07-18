#include "pectoral_control.h"

CPGModel lpect_flap_cpg_model;
CPGModel rpect_flap_cpg_model;
CPGModel lpect_feather_cpg_model;
CPGModel rpect_feather_cpg_model;
CPGModel lpect_pitch_cpg_model;
CPGModel rpect_pitch_cpg_model;

//胸鳍控制
void pectoral_fin_ctrl(float angle, uint8_t channel){
    float pwm_to_set = 0;

    if(channel == RPECT_FLAP || channel == RPECT_FEATHER || channel == RPECT_PITCH){
            angle = -angle;
    }
//  printf("angle %f\n",angle);
//  printf("mid_pwm %d\n",SERVO_MID_PWM);
//  printf("pwm_per_rad %d\n",PWM_PER_RAD);

    switch(channel){
        case LPECT_FLAP:
            pwm_to_set = SERVO_MID_PWM + (float)(FLAP_PWM_PER_RAD*angle);
            robopenguin.swim_param.pect_flap_angle[0] = angle;
            servo_pwm_set_compare(pwm_to_set,PWM3);
//          printf("angle %f\n",angle);
            //printf("pwm %f\n",pwm_to_set);
          break;
        case RPECT_FLAP:
            pwm_to_set = SERVO_MID_PWM + (float)(FLAP_PWM_PER_RAD*angle);
            robopenguin.swim_param.pect_flap_angle[1] = angle;
            servo_pwm_set_compare(pwm_to_set,PWM4);
          break;
        case LPECT_FEATHER:
            pwm_to_set = SERVO_MID_PWM + (float)(FEATHER_PWM_PER_RAD*angle);
            robopenguin.swim_param.pect_feather_angle[0] = angle;
            servo_pwm_set_compare(pwm_to_set,PWM5);
          break;
        case RPECT_FEATHER:
            pwm_to_set = SERVO_MID_PWM + (float)(FEATHER_PWM_PER_RAD*angle);
            robopenguin.swim_param.pect_feather_angle[1] = angle;
            servo_pwm_set_compare(pwm_to_set,PWM6);
          break;
        case LPECT_PITCH:
            pwm_to_set = 1580 + (float)(PITCH_PWM_PER_RAD*angle);
            robopenguin.swim_param.pect_pitch_angle[0] = angle;
            servo_pwm_set_compare(pwm_to_set,PWM7);
          break;
        case RPECT_PITCH:
            pwm_to_set = 1500 + (float)(PITCH_PWM_PER_RAD*angle);
            robopenguin.swim_param.pect_pitch_angle[1] = angle;
            servo_pwm_set_compare(pwm_to_set,PWM8);
          break;
        case TAIL:
            pwm_to_set = 1420 + (float)(TAIL_PWM_PER_RAD*angle);
            robopenguin.swim_param.tail_angle = angle;
            servo_pwm_set_compare(pwm_to_set,PWM9);
          break;
        default:
            break;
    }
}

void pectoral_fin_init(void){
        //胸鳍flap参数初始化
        robopenguin.swim_param.pect_flap_amp[0] = 0.0f;
        robopenguin.swim_param.pect_flap_amp[1] = 0.0f;
        robopenguin.swim_param.pect_flap_freq[0] = 0.0f;
        robopenguin.swim_param.pect_flap_freq[1] = 0.0f;
        robopenguin.swim_param.pect_flap_offset[0] = 0.0f;
        robopenguin.swim_param.pect_flap_offset[1] = 0.0f;

        #ifdef USE_CPG
        CPG_InitModelDefault(&lpect_flap_cpg_model);//初始化cpg结构体中的参数
        CPG_InitModelDefault(&rpect_flap_cpg_model);//初始化cpg结构体中的参数
        CPG_SetAmplitude(&lpect_flap_cpg_model, 1);
        CPG_SetFrequency(&lpect_flap_cpg_model, 0.3);
        CPG_SetOffset(&lpect_flap_cpg_model, 0);
        CPG_SetAmplitude(&rpect_flap_cpg_model, 0);
        CPG_SetFrequency(&rpect_flap_cpg_model, 0);
        CPG_SetOffset(&rpect_flap_cpg_model, 0);
        #else
        SetSINOffset(&lpect_flap_sin_model, 0);
        SetSINAmp(&lpect_flap_sin_model, 0);
        SetSINFreq(&lpect_flap_sin_model, 0);//30->2.8 40->2.2 50->1.8
        SetSINOffset(&rpect_flap_sin_model, 0);
        SetSINAmp(&rpect_flap_sin_model, 0);
        SetSINFreq(&rpect_flap_sin_model, 0);
        #endif

        //胸鳍feather参数初始化
        robopenguin.swim_param.pect_feather_amp[0] = 0.0f;
        robopenguin.swim_param.pect_feather_amp[1] = 0.0f;
        robopenguin.swim_param.pect_feather_freq[0] = 0.0f;
        robopenguin.swim_param.pect_feather_freq[1] = 0.0f;
        robopenguin.swim_param.pect_feather_offset[0] = 0.0f;
        robopenguin.swim_param.pect_feather_offset[1] = 0.0f;

        #ifdef USE_CPG
        CPG_InitModelDefault(&lpect_feather_cpg_model);//初始化cpg结构体中的参数
        CPG_InitModelDefault(&rpect_feather_cpg_model);//初始化cpg结构体中的参数
        CPG_SetAmplitude(&lpect_feather_cpg_model, 0);
        CPG_SetFrequency(&lpect_feather_cpg_model, 0);
        CPG_SetOffset(&lpect_feather_cpg_model, 0);
        CPG_SetAmplitude(&rpect_feather_cpg_model, 0);
        CPG_SetFrequency(&rpect_feather_cpg_model, 0);
        CPG_SetOffset(&rpect_feather_cpg_model, 0);
        #else
        SetSINOffset(&lpect_feather_sin_model, 0);
        SetSINAmp(&lpect_feather_sin_model, 0);
        SetSINFreq(&lpect_feather_sin_model, 0);
        SetSINOffset(&rpect_feather_sin_model, 0);
        SetSINAmp(&rpect_feather_sin_model, 0);
        SetSINFreq(&rpect_feather_sin_model, 0);
        #endif

        //胸鳍pitch参数初始化
        robopenguin.swim_param.pect_pitch_amp[0] = 0.0f;
        robopenguin.swim_param.pect_pitch_amp[1] = 0.0f;
        robopenguin.swim_param.pect_pitch_freq[0] = 0.0f;
        robopenguin.swim_param.pect_pitch_freq[1] = 0.0f;
        robopenguin.swim_param.pect_pitch_offset[0] = 0.0f;
        robopenguin.swim_param.pect_pitch_offset[1] = 0.0f;

        #ifdef USE_CPG
        CPG_InitModelDefault(&lpect_pitch_cpg_model);//初始化cpg结构体中的参数
        CPG_InitModelDefault(&rpect_pitch_cpg_model);//初始化cpg结构体中的参数
        CPG_SetAmplitude(&lpect_pitch_cpg_model, 0);
        CPG_SetFrequency(&lpect_pitch_cpg_model, 0);
        CPG_SetOffset(&lpect_pitch_cpg_model, 0);
        CPG_SetAmplitude(&rpect_pitch_cpg_model, 0);
        CPG_SetFrequency(&rpect_pitch_cpg_model, 0);
        CPG_SetOffset(&rpect_pitch_cpg_model, 0);
        #else
        SetSINOffset(&lpect_pitch_sin_model, 0);
        SetSINAmp(&lpect_pitch_sin_model, 0);
        SetSINFreq(&lpect_pitch_sin_model, 0);
        SetSINOffset(&rpect_pitch_sin_model, 0);
        SetSINAmp(&rpect_pitch_sin_model, 0);
        SetSINFreq(&rpect_pitch_sin_model, 0);
        #endif

}


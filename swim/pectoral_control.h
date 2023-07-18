#ifndef _PECTORAL_CTRL_H
#define _PECTORAL_CTRL_H

#include <rtthread.h>
#include <stdbool.h>

#include "pwm.h"
#include "delay.h"

#include "sin.h"
#include "cpg.h"
#include "robotstate.h"

extern CPGModel lpect_flap_cpg_model;
extern CPGModel rpect_flap_cpg_model;
extern CPGModel lpect_feather_cpg_model;
extern CPGModel rpect_feather_cpg_model;
extern CPGModel lpect_pitch_cpg_model;
extern CPGModel rpect_pitch_cpg_model;

#define LPECT_FLAP     0
#define RPECT_FLAP     1
#define LPECT_FEATHER  2
#define RPECT_FEATHER  3
#define LPECT_PITCH    4
#define RPECT_PITCH    5
#define TAIL           6

#define FEATHER_PWM_PER_RAD    424 //40бу
#define FLAP_PWM_PER_RAD       390 //40бу
#define PITCH_PWM_PER_RAD      424 //30бу
#define TAIL_PWM_PER_RAD       390 //30бу
#define SERVO_MID_PWM  1500

void pectoral_fin_ctrl(float angle, uint8_t channel);
void pectoral_fin_init(void);

#endif

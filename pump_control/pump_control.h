#ifndef _PUMP_CONTROL_H
#define _PUMP_CONTROL_H

#include <rtthread.h>
#include <stdbool.h>
#include "delay.h"
#include "robotstate.h"
//#include "utils.h"
//#include "application.h"
#include "pwm.h"
#include "pump_angle_iic.h"
#include "pump_angle.h"

#define pump_mid_position 7
#define pump_bm_position 3
#define pump_fm_position 11

//#define PUMP_FULL_ANGLE 450
//#define PUMP_MID_ANGLE 450

extern unsigned short int length_aveval_pump;
extern int pump_distance;

//extern float limit_switch_angle;
//extern int limit_switch_count;
//extern float pump_ori_angle;
//extern float pump_last_angle;
//extern float pump_final_angle;
//extern float pump_count;

static void pump_move_entry(void* parameter);
void pump_move_thread_create(void);

//static void pump_init_entry(void* parameter);
//void pump_init_thread_create(void);
//void pump_init(void);
//void _pump_init(void);
//void set_pump_position(int position);

void pump_in(void);
void pump_out(void);
void pump_off(void);

void pump_test(void);

#endif


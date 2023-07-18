#ifndef _MASS_CTRL_H
#define _MASS_CTRL_H

#include <rtthread.h>
#include <stdbool.h>
#include "delay.h"
#include "robotstate.h"
//#include "utils.h"
//#include "application.h"
#include "pwm.h"
#include "mass_angle_iic.h"
#include "mass_angle.h"

//#define MASS_FULL_ANGLE 1100

#define mass_mid_position 7
#define mass_bm_position 13
#define mass_fm_position 3

//#define mass_mid_position 180
//#define mass_max_position 360

//extern float mass_limit_switch_angle;
//extern int mass_limit_switch_count;

extern unsigned short int length_aveval_mass;
extern int mass_distance;

extern int mass_moving_direction;

extern int mass_cnt;


//void mass_move_thread_create(void);
//static void mass_ccontrol_thread_entry(void* parameter);
void mass_init(void);
void _mass_init(void);
//void set_mass_position(int position);

void mass_control_thread_create(void);
void mass_control_entry(void* parameter);

void mass_control(void);
void mass_control_over(void);

void mass_test(void);

void mass_stop(void);
void mass_fmove(void);
void mass_bmove(void);


#endif

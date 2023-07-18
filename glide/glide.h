#ifndef _GLIDE_H
#define _GLIDE_H

#include "robotstate.h"
#include "delay.h"
#include <rtthread.h>
#include <stdbool.h>
#include "pwm.h"
#include "pectoral_control.h"

//首先定义PID结构体用于存放一个PID的数据
typedef struct
{
    float kp,ki,kd;//三个系数
    float error,lastError;//误差、上次误差
    float integral,maxIntegral;//积分、积分限幅
    float output,maxOutput;//输出、输出限幅
}PID;

//串级PID的结构体，包含两个单级PID
typedef struct
{
    PID inner;//内环
    PID outer;//外环
    float output;//串级输出，等于inner.output
}CascadePID;

static void glide_entry(void* parameter);
void glide_thread_create(void);

void PID_CascadeCalc(CascadePID *pid,float outerRef,float outerFdb,float innerFdb);
void PID_Calc(PID *pid,float reference,float feedback);
void PID_Init(PID *pid,float p,float i,float d,float maxI,float maxOut);

#endif

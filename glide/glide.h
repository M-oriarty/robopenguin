#ifndef _GLIDE_H
#define _GLIDE_H

#include "robotstate.h"
#include "delay.h"
#include <rtthread.h>
#include <stdbool.h>
#include "pwm.h"
#include "pectoral_control.h"

//���ȶ���PID�ṹ�����ڴ��һ��PID������
typedef struct
{
    float kp,ki,kd;//����ϵ��
    float error,lastError;//���ϴ����
    float integral,maxIntegral;//���֡������޷�
    float output,maxOutput;//���������޷�
}PID;

//����PID�Ľṹ�壬������������PID
typedef struct
{
    PID inner;//�ڻ�
    PID outer;//�⻷
    float output;//�������������inner.output
}CascadePID;

static void glide_entry(void* parameter);
void glide_thread_create(void);

void PID_CascadeCalc(CascadePID *pid,float outerRef,float outerFdb,float innerFdb);
void PID_Calc(PID *pid,float reference,float feedback);
void PID_Init(PID *pid,float p,float i,float d,float maxI,float maxOut);

#endif

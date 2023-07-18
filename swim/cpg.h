#ifndef __CPG_H
#define __CPG_H

#include <rtthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "math.h"

#define PI 3.14

typedef struct
{
    //Inputs
    float target_amp;//unit:rad
    float target_freq;//unit:Hz
    float offset;//unit:rad

    //Outputs
    float angle;//unit:rad

    //Private params
    float smooth_freq;//unit:Hz
    float hopfcpg_deltax;//中间变量
    float hopfcpg_deltay;//中间变量
    float hopfcpg_alpha;//控制极限环的收敛速度
    float hopfcpg_miu;//控制信号幅度，A=miu的开方
    float hopfcpg_x;//x相
    float hopfcpg_y;//y相
}CPGModel;


void CPG_InitModelDefault(CPGModel *model);
void CPG_Reset(CPGModel *model);
void CPG_SetOffset(CPGModel *model, float offset);
void CPG_SetAmplitude(CPGModel *model, float amp);
void CPG_SetFrequency(CPGModel *model, float freq);
void CPG_UpdateModel(CPGModel *model, float timespan);


#endif

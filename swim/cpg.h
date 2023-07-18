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
    float hopfcpg_deltax;//�м����
    float hopfcpg_deltay;//�м����
    float hopfcpg_alpha;//���Ƽ��޻��������ٶ�
    float hopfcpg_miu;//�����źŷ��ȣ�A=miu�Ŀ���
    float hopfcpg_x;//x��
    float hopfcpg_y;//y��
}CPGModel;


void CPG_InitModelDefault(CPGModel *model);
void CPG_Reset(CPGModel *model);
void CPG_SetOffset(CPGModel *model, float offset);
void CPG_SetAmplitude(CPGModel *model, float amp);
void CPG_SetFrequency(CPGModel *model, float freq);
void CPG_UpdateModel(CPGModel *model, float timespan);


#endif

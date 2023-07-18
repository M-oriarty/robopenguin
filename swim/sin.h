#ifndef __SIN_H
#define __SIN_H

#include <string.h>
#include <stdlib.h>
#include "math.h"
#include "stdio.h"
#include "pectoral_control.h"

#ifndef PI
    #define PI (3.141592653f)
#endif

#define RAD2DEG(x) ((x)*180.f/PI)
#define DEG2RAD(x) ((x)*PI/180.f)

typedef struct
{
    //input
    float amp;//unit:rad
    float freq;//unit:Hz
    float offset;//unit:rad
    //output
    float angle;//unit:rad
    float angleVel;//unit:rad/s
}SINModel;//单关节机器鱼，幅值、频率、偏置

void InitSINModel(SINModel* sinmodel, float amp, float freq, float offset);
void CalJtAng(SINModel* sinmodel, float t);
void SetSINAmp(SINModel* sinmodel, float amp);
void SetSINOffset(SINModel* sinmodel, float offset);
void SetSINFreq(SINModel* sinmodel, float freq);
void RetDefaultSIN(SINModel* sinmodel);

extern SINModel lpect_flap_sin_model;
extern SINModel rpect_flap_sin_model;
extern SINModel lpect_feather_sin_model;
extern SINModel rpect_feather_sin_model;
extern SINModel lpect_pitch_sin_model;
extern SINModel rpect_pitch_sin_model;
extern SINModel tail_sin_model;


#endif

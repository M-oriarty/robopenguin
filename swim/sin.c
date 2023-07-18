#include "sin.h"

#define BW_INIT_FREQ             (0.0f)//body wave init frequency
#define BW_INIT_AMP              (DEG2RAD(0.0f))//body wave init amplitude
#define BW_INIT_OFFSET           (0)//body wave init offset

SINModel lpect_flap_sin_model;
SINModel rpect_flap_sin_model;
SINModel lpect_feather_sin_model;
SINModel rpect_feather_sin_model;
SINModel lpect_pitch_sin_model;
SINModel rpect_pitch_sin_model;
SINModel tail_sin_model;

void CalJtAng(SINModel* sinmodel, float t)//正弦波运动方程
{
    float omega = 2*PI*sinmodel->freq;
    float amp = sinmodel->amp;
    float offset = sinmodel->offset;
    sinmodel->angle = amp * sinf(omega*t) + offset;
    if(sinmodel == &lpect_flap_sin_model){
//      printf("freq %f\n",sinmodel->freq);
//      printf("amp %f\n",sinmodel->amp);
//      printf("offset %f\n",sinmodel->offset);
//      printf("omega %f\n",omega);
//      printf("t %f\n",t);
//      printf("sin %f\n",arm_sin_f32(omega*t));
//      printf("angle %f\n",sinmodel->angle);
    }
    sinmodel->angleVel = omega * amp * cosf(omega*t);
}

void SetSINFreq(SINModel* sinmodel, float freq)
{
    sinmodel->freq = freq;
}


void SetSINOffset(SINModel* sinmodel, float offset)
{
    sinmodel->offset = offset;
}


void SetSINAmp(SINModel* sinmodel, float amp)
{
    sinmodel->amp = amp;
}


void RetDefaultSIN(SINModel* sinmodel)
{
    SetSINAmp(sinmodel, BW_INIT_AMP);

    SetSINOffset(sinmodel, BW_INIT_OFFSET);

    SetSINFreq(sinmodel, BW_INIT_FREQ);
}


void InitSINModel(SINModel* sinmodel, float amp, float freq, float offset)
{
    SetSINAmp(sinmodel, amp);

    SetSINOffset(sinmodel, offset);

    SetSINFreq(sinmodel, freq);
}

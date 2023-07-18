#include "cpg.h"


const float CPGCalcStepSize = 0.001f;//unit:s
const float MaxDeltaFreq = 0.01f;//unit:Hz
const float ConvergenceSpeed = 5.0f;

const float DefaultFreq = 0.0f;//unit:Hz
const float DefaultAmp = 0.0f;//unit:rad
const float DefaultOffset = 0.0f;//unit:rad


/*------------------------------------------------------------
 *@brief Update the CPG model for one single calculation step
 *
 *@param model
 *@param timestep
 *
 *HOPF振荡器变量及参数定义
 *deltax = alpha*(miu-r*r)*x-omega*y
 *deltay = alpha*(miu-r*r)*y+omega*x
 *其中r=x*x+y*y
 *-----------------------------------------------------------*/
void CPG_UpdateModelOneStep(CPGModel *model)
{
    //该函数内部用到的局部参量（中间变量）
    float omega;
    float delta_freq;
    float timestep = CPGCalcStepSize;

    //平滑
    #ifdef CPG_FREQ_SMOOTH_ON//如果开启CPG平滑模式
        if(model->smooth_freq < model->target_freq)//如果平滑频率小于目标频率
        {
            delta_freq = timestep * MaxDeltaFreq;//单位频率=时间步长*最大单位频率，频率增大的时候才起作用？？？？？？
            if(fabs(model->smooth_freq - model->target_freq) > delta_freq)//如果两者之差大于单位频率
            {
                model->smooth_freq += delta_freq * Sign(model->target_freq - model->smooth_freq);//重新计算平滑频率
            }
            else
            {
                model->smooth_freq = model->target_freq;
            }
        }
        else
        {
            model->smooth_freq = model->target_freq;//如果平滑频率大于等于目标频率，不变，意味着频率往小了去不管频率突变了
        }
    #else
        model->smooth_freq = model->target_freq;//如果不开启CPG平滑模式，不变
    #endif

    //omega的计算
    omega = 2*PI*model->smooth_freq;

    //HOPF振荡器
    model->hopfcpg_deltax = (model->hopfcpg_alpha * (model->hopfcpg_miu
        - (model->hopfcpg_x*model->hopfcpg_x+model->hopfcpg_y*model->hopfcpg_y))
        * model->hopfcpg_x - omega * model->hopfcpg_y) * timestep;
    model->hopfcpg_deltay = (model->hopfcpg_alpha * (model->hopfcpg_miu
        - (model->hopfcpg_x*model->hopfcpg_x+model->hopfcpg_y*model->hopfcpg_y))
        * model->hopfcpg_y + omega * model->hopfcpg_x) * timestep;
    model->hopfcpg_x = model->hopfcpg_x + model->hopfcpg_deltax;
    model->hopfcpg_y = model->hopfcpg_y + model->hopfcpg_deltay;
}


/*------------------------------------------------------------
 *@brief Update the CPG model for multi calculation step
 *
 *@param model
 *@param timestep
 *@param timespan
 *
 *-----------------------------------------------------------*/
void CPG_UpdateModelMultiSteps(CPGModel *model, float timespan)
{
    float timestep = CPGCalcStepSize;
    while(timespan > 0)
    {
        if(timespan >= timestep)
        {
            CPG_UpdateModelOneStep(model);
            timespan -= timestep;
        }
        else
        {
            CPG_UpdateModelOneStep(model);
            break;
        }
    }
    model->angle = model->hopfcpg_y + model->offset;
}


/*------------------------------------------------------------
 *@brief Update the CPG model 接口函數
 *
 *@param model
 *@param timespan
 *
 *-----------------------------------------------------------*/
void CPG_UpdateModel(CPGModel *model, float timespan)//unit:s
{
    CPG_UpdateModelMultiSteps(model, timespan);
}


/*------------------------------------------------------------
 *@brief Set the frequency of CPG Model 接口函數
 *
 *@param model
 *@param freq
 *
 *-----------------------------------------------------------*/
void CPG_SetFrequency(CPGModel *model, float freq)//unit:Hz
{
    model->target_freq = freq;
}


/*------------------------------------------------------------
 *@brief Set the amplitude of CPG Model 接口函數
 *
 *@param model
 *@param amp
 *
 *-----------------------------------------------------------*/
void CPG_SetAmplitude(CPGModel *model, float amp)//unit:rad
{
    model->target_amp = amp;
    model->hopfcpg_miu = (model->target_amp*model->target_amp);
}


/*------------------------------------------------------------
 *@brief Set the offset of CPG Model 接口函數
 *
 *@param model
 *@param offset
 *
 *-----------------------------------------------------------*/
void CPG_SetOffset(CPGModel *model, float offset)//unit:rad
{
    model->offset = offset;
}


/*------------------------------------------------------------
 *@brief Reset all the middle data in CPG Model 接口函數
 *
 *@param model
 *
 *-----------------------------------------------------------*/
void CPG_Reset(CPGModel *model)
{
    //机器鱼摆动参数相关
    model->target_amp = DefaultAmp;
    model->smooth_freq = DefaultFreq;
    model->offset = DefaultOffset;
    //HopfCPG相关
    model->hopfcpg_miu = (model->target_amp*model->target_amp);
    model->hopfcpg_alpha = ConvergenceSpeed;
    model->hopfcpg_deltax = 0;
    model->hopfcpg_deltay = 0;
    model->hopfcpg_x = 1;
    model->hopfcpg_y = 0;
}


/*------------------------------------------------------------
 *@brief Init the CPG Model with the default params 接口函數
 *
 *@param model
 *
 *-----------------------------------------------------------*/
void CPG_InitModelDefault(CPGModel *model)
{
    //存储空间
    memset(model, 0, sizeof(CPGModel));
    //机器鱼摆动参数相关
    model->target_amp = DefaultAmp;
    model->smooth_freq = DefaultFreq;
    model->offset = DefaultOffset;
    //HopfCPG参数相关
    model->hopfcpg_miu = (model->target_amp*model->target_amp);
    model->hopfcpg_alpha = ConvergenceSpeed;
    model->hopfcpg_deltax = 0;
    model->hopfcpg_deltay = 0;
    model->hopfcpg_x = 1;
    model->hopfcpg_y = 0;
}

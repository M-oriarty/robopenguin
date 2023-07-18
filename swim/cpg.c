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
 *HOPF������������������
 *deltax = alpha*(miu-r*r)*x-omega*y
 *deltay = alpha*(miu-r*r)*y+omega*x
 *����r=x*x+y*y
 *-----------------------------------------------------------*/
void CPG_UpdateModelOneStep(CPGModel *model)
{
    //�ú����ڲ��õ��ľֲ��������м������
    float omega;
    float delta_freq;
    float timestep = CPGCalcStepSize;

    //ƽ��
    #ifdef CPG_FREQ_SMOOTH_ON//�������CPGƽ��ģʽ
        if(model->smooth_freq < model->target_freq)//���ƽ��Ƶ��С��Ŀ��Ƶ��
        {
            delta_freq = timestep * MaxDeltaFreq;//��λƵ��=ʱ�䲽��*���λƵ�ʣ�Ƶ�������ʱ��������ã�����������
            if(fabs(model->smooth_freq - model->target_freq) > delta_freq)//�������֮����ڵ�λƵ��
            {
                model->smooth_freq += delta_freq * Sign(model->target_freq - model->smooth_freq);//���¼���ƽ��Ƶ��
            }
            else
            {
                model->smooth_freq = model->target_freq;
            }
        }
        else
        {
            model->smooth_freq = model->target_freq;//���ƽ��Ƶ�ʴ��ڵ���Ŀ��Ƶ�ʣ����䣬��ζ��Ƶ����С��ȥ����Ƶ��ͻ����
        }
    #else
        model->smooth_freq = model->target_freq;//���������CPGƽ��ģʽ������
    #endif

    //omega�ļ���
    omega = 2*PI*model->smooth_freq;

    //HOPF����
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
 *@brief Update the CPG model �ӿں���
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
 *@brief Set the frequency of CPG Model �ӿں���
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
 *@brief Set the amplitude of CPG Model �ӿں���
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
 *@brief Set the offset of CPG Model �ӿں���
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
 *@brief Reset all the middle data in CPG Model �ӿں���
 *
 *@param model
 *
 *-----------------------------------------------------------*/
void CPG_Reset(CPGModel *model)
{
    //������ڶ��������
    model->target_amp = DefaultAmp;
    model->smooth_freq = DefaultFreq;
    model->offset = DefaultOffset;
    //HopfCPG���
    model->hopfcpg_miu = (model->target_amp*model->target_amp);
    model->hopfcpg_alpha = ConvergenceSpeed;
    model->hopfcpg_deltax = 0;
    model->hopfcpg_deltay = 0;
    model->hopfcpg_x = 1;
    model->hopfcpg_y = 0;
}


/*------------------------------------------------------------
 *@brief Init the CPG Model with the default params �ӿں���
 *
 *@param model
 *
 *-----------------------------------------------------------*/
void CPG_InitModelDefault(CPGModel *model)
{
    //�洢�ռ�
    memset(model, 0, sizeof(CPGModel));
    //������ڶ��������
    model->target_amp = DefaultAmp;
    model->smooth_freq = DefaultFreq;
    model->offset = DefaultOffset;
    //HopfCPG�������
    model->hopfcpg_miu = (model->target_amp*model->target_amp);
    model->hopfcpg_alpha = ConvergenceSpeed;
    model->hopfcpg_deltax = 0;
    model->hopfcpg_deltay = 0;
    model->hopfcpg_x = 1;
    model->hopfcpg_y = 0;
}

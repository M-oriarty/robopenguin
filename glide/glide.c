#include "glide.h"

//���ڳ�ʼ��pid�����ĺ���
void PID_Init(PID *pid,float p,float i,float d,float maxI,float maxOut)
{
    pid->kp=p;
    pid->ki=i;
    pid->kd=d;
    pid->maxIntegral=maxI;
    pid->maxOutput=maxOut;
}

//����һ��pid����
//����Ϊ(pid�ṹ��,Ŀ��ֵ,����ֵ)������������pid�ṹ���output��Ա��
void PID_Calc(PID *pid,float reference,float feedback)
{
    //��������
    pid->lastError=pid->error;//����error������
    pid->error=reference-feedback;//������error
    //����΢��
    float dout=(pid->error-pid->lastError)*pid->kd;
    //�������
    float pout=pid->error*pid->kp;
    //�������
    pid->integral+=pid->error*pid->ki;
    //�����޷�
    if(pid->integral > pid->maxIntegral) pid->integral=pid->maxIntegral;
    else if(pid->integral < -pid->maxIntegral) pid->integral=-pid->maxIntegral;
    //�������
    pid->output=pout+dout+pid->integral;
    //����޷�
    if(pid->output > pid->maxOutput) pid->output=pid->maxOutput;
    else if(pid->output < -pid->maxOutput) pid->output=-pid->maxOutput;
}



//����PID�ļ��㺯��
//����(PID�ṹ��,�⻷Ŀ��ֵ,�⻷����ֵ,�ڻ�����ֵ)
void PID_CascadeCalc(CascadePID *pid,float outerRef,float outerFdb,float innerFdb)
{
    PID_Calc(&pid->outer,outerRef,outerFdb);//�����⻷
    PID_Calc(&pid->inner,pid->outer.output,innerFdb);//�����ڻ�
    pid->output=pid->inner.output;//�ڻ�������Ǵ���PID�����
}

//CascadePID glide_pid;//��������PID�ṹ�����
PID glide_pid;


#define GLIDE_THREAD_PRIORITY         25
#define GLIDE_THREAD_STACK_SIZE       1024


/*��ʹ�þ�̬�߳�ʱ���̵߳�ջ��Ҫ�����ֽڶ��롡*/
ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t glide_thread_stack[GLIDE_THREAD_STACK_SIZE];
static struct rt_thread glide_thread;

static void glide_entry(void* parameter){
    //PID_Init(&glide_pid.inner,10,0,0,0,1000);//��ʼ���ڻ�����
    //PID_Init(&glide_pid.outer,5,0,5,0,100);//��ʼ���⻷����
    PID_Init(&glide_pid,10,0,5,20,200);
    while(1){
        //float outerTarget=PI/6;//��ȡ�⻷Ŀ��ֵ
        //float outerFeedback=robopenguin.imu_data.pitch;//��ȡ�⻷����ֵ
        //float innerFeedback= PWM;//��ȡ�ڻ�����ֵ
        //PID_CascadeCalc(&glide_pid,outerTarget,outerFeedback,innerFeedback);//����PID����
        float reference = PI/6;
        float feedback = robopenguin.imu_data.pitch;
        PID_Calc(&glide_pid,reference,feedback);
        int compareValue = 1500 + glide_pid.output;
    }
}

void glide_thread_create(void)
{

    rt_thread_init(&glide_thread,
                                    "GLIDE",
                                    glide_entry,
                                    RT_NULL,
                                    glide_thread_stack,
                                    GLIDE_THREAD_STACK_SIZE,
                                    GLIDE_THREAD_PRIORITY,30);

    rt_thread_startup(&glide_thread);

}

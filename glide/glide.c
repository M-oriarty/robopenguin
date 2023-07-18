#include "glide.h"

//用于初始化pid参数的函数
void PID_Init(PID *pid,float p,float i,float d,float maxI,float maxOut)
{
    pid->kp=p;
    pid->ki=i;
    pid->kd=d;
    pid->maxIntegral=maxI;
    pid->maxOutput=maxOut;
}

//进行一次pid计算
//参数为(pid结构体,目标值,反馈值)，计算结果放在pid结构体的output成员中
void PID_Calc(PID *pid,float reference,float feedback)
{
    //更新数据
    pid->lastError=pid->error;//将旧error存起来
    pid->error=reference-feedback;//计算新error
    //计算微分
    float dout=(pid->error-pid->lastError)*pid->kd;
    //计算比例
    float pout=pid->error*pid->kp;
    //计算积分
    pid->integral+=pid->error*pid->ki;
    //积分限幅
    if(pid->integral > pid->maxIntegral) pid->integral=pid->maxIntegral;
    else if(pid->integral < -pid->maxIntegral) pid->integral=-pid->maxIntegral;
    //计算输出
    pid->output=pout+dout+pid->integral;
    //输出限幅
    if(pid->output > pid->maxOutput) pid->output=pid->maxOutput;
    else if(pid->output < -pid->maxOutput) pid->output=-pid->maxOutput;
}



//串级PID的计算函数
//参数(PID结构体,外环目标值,外环反馈值,内环反馈值)
void PID_CascadeCalc(CascadePID *pid,float outerRef,float outerFdb,float innerFdb)
{
    PID_Calc(&pid->outer,outerRef,outerFdb);//计算外环
    PID_Calc(&pid->inner,pid->outer.output,innerFdb);//计算内环
    pid->output=pid->inner.output;//内环输出就是串级PID的输出
}

//CascadePID glide_pid;//创建串级PID结构体变量
PID glide_pid;


#define GLIDE_THREAD_PRIORITY         25
#define GLIDE_THREAD_STACK_SIZE       1024


/*　使用静态线程时，线程的栈需要设置字节对齐　*/
ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t glide_thread_stack[GLIDE_THREAD_STACK_SIZE];
static struct rt_thread glide_thread;

static void glide_entry(void* parameter){
    //PID_Init(&glide_pid.inner,10,0,0,0,1000);//初始化内环参数
    //PID_Init(&glide_pid.outer,5,0,5,0,100);//初始化外环参数
    PID_Init(&glide_pid,10,0,5,20,200);
    while(1){
        //float outerTarget=PI/6;//获取外环目标值
        //float outerFeedback=robopenguin.imu_data.pitch;//获取外环反馈值
        //float innerFeedback= PWM;//获取内环反馈值
        //PID_CascadeCalc(&glide_pid,outerTarget,outerFeedback,innerFeedback);//进行PID计算
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

#include "data_show_app.h"
#define DATA_SHOW_PERIOD  (500)//unit:ms

extern ROBOPENGUIN robopenguin;

ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t data_show_thread_stack[1024*10];
static struct rt_thread data_show_thread;//�ζ������߳�
static struct rt_timer data_show_timer;//�ζ����ƶ�ʱ��

struct rt_semaphore data_show_sem;//�ζ������߳��ź���

static Command s_datatype; //��������
static uint8_t s_dataaxis; //��������(X, Y, Z)

//�����������ͺ���������ӿں���
void set_data_type(Command command, uint8_t data)
{
    s_datatype = command;
    s_dataaxis = data;
}

/*--------------------------------���������ݸ����߳�-------------------------------*/
float timer_os_tick2;
//��ʱ���ͷ��ź��������߳�
void data_show_thread_tmrcallback(void* parameter)
{
    rt_tick_t ti;
    ti = rt_tick_get();
    timer_os_tick2 = (float)(ti)/(float)(RT_TICK_PER_SECOND);
    rt_sem_release(&data_show_sem);
    //rt_thread_resume(&data_show_thread);
}

//����չʾ�߳�
static void data_show_thread_entry(void* parameter)
{
    //���������ݻ���
    uint8_t databuf[12];
    //����ѭ��
    while(1)
    {
        if(rt_sem_take(&data_show_sem, RT_WAITING_FOREVER) ==  RT_EOK)//�ж��ź����Ƿ����ʹ��
        {
            switch(s_datatype){
                case READ_DEPTH:
                    memcpy(&databuf[0],&robopenguin.depth,4);
                    break;

                case READ_IMU_ATTITUDE:
                    switch(s_dataaxis){
                        case 1:
                            memcpy(&databuf[0],&robopenguin.imu_data.pitch,4);
                            break;
                        case 2:
                            memcpy(&databuf[0],&robopenguin.imu_data.yaw,4);
                            break;
                        case 3:
                            memcpy(&databuf[0],&robopenguin.imu_data.roll,4);
                            break;
                    }
                    break;

                case READ_IMU_ACCEL:
                    switch(s_dataaxis){
                        case 1:
                            memcpy(&databuf[0],&robopenguin.imu_data.accelx,4);
                            break;
                        case 2:
                            memcpy(&databuf[0],&robopenguin.imu_data.accely,4);
                            break;
                        case 3:
                            memcpy(&databuf[0],&robopenguin.imu_data.accelz,4);
                            break;
                    }
                    break;

                    case READ_IMU_GYRO:
                        switch(s_dataaxis){
                            case 1:
                            memcpy(&databuf[0],&robopenguin.imu_data.gyrox,4);
                            break;
                        case 2:
                            memcpy(&databuf[0],&robopenguin.imu_data.gyroy,4);
                            break;
                        case 3:
                            memcpy(&databuf[0],&robopenguin.imu_data.gyroz,4);
                            break;
                        }
                        break;

                    case READ_CURRENT:
                        switch(s_dataaxis){
                            case 1:
                            memcpy(&databuf[0],&robopenguin.power_data.Current_data[0],4);
                            break;
                            case 2:
                            memcpy(&databuf[0],&robopenguin.power_data.Current_data[1],4);
                            break;
                        }
                        break;

                    case READ_VOLTAGE:
                        switch(s_dataaxis){
                            case 1:
                            memcpy(&databuf[0],&robopenguin.power_data.Voltage_data[0],4);
                            break;
                            case 2:
                            memcpy(&databuf[0],&robopenguin.power_data.Voltage_data[1],4);
                            break;
                        }
                        break;



                    case READ_POWER:
                        switch(s_dataaxis){
                            case 1:
                            memcpy(&databuf[0],&robopenguin.power_data.Power_data[0],4);
                            break;
                            case 2:
                            memcpy(&databuf[0],&robopenguin.power_data.Power_data[1],4);
                            break;
                        }
                        break;

                    default:
                        break;
            }
            //�����Э�鷢������
            RFLink_sendStruct(FRIEND_ID1, s_datatype, databuf, 4);
            rt_thread_delay(50);
        }
        rt_thread_delay(5);
    }
}

//��������չʾ�߳�
void data_show_thread_create(void)
{
    rt_err_t result_app;

    result_app = rt_thread_init(&data_show_thread,
                                 "data show thread",
                                  data_show_thread_entry,
                                  RT_NULL,
                                 (rt_uint8_t*)&data_show_thread_stack[0],
                                  sizeof(data_show_thread_stack),
                                                                25,50);//���Tickûɶ�ã���Ҫ���ÿ�rt_thread_delay

    rt_timer_init(&data_show_timer,
                                 "_data_show_timer",
                                  data_show_thread_tmrcallback,
                                  RT_NULL,
                              DATA_SHOW_PERIOD,  //unit:ms
                                  RT_TIMER_FLAG_PERIODIC);//�����Զ�ʱ��
    rt_sem_init(&data_show_sem, "data_show_sem", 0, RT_IPC_FLAG_FIFO);//��ʼ��һ���ź���
  if(result_app == RT_EOK)
    {
        rt_thread_startup(&data_show_thread);//�����߳�
    }
//  rt_timer_start(&data_show_timer);//������ʱ��(����رշ��Ͳ���Ҫֹͣ��ʱ��������ֹͣ��ʱ�����ô�֮���ٿ���)
}

//��ͣ���нӿں���
void data_show_app_stop(void)
{
    rt_timer_stop(&data_show_timer);
}

//�ָ����нӿں���
void data_show_app_resume(void)
{
    rt_timer_start(&data_show_timer);
}

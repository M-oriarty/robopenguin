#include "data_show_app.h"
#define DATA_SHOW_PERIOD  (500)//unit:ms

extern ROBOPENGUIN robopenguin;

ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t data_show_thread_stack[1024*10];
static struct rt_thread data_show_thread;//游动控制线程
static struct rt_timer data_show_timer;//游动控制定时器

struct rt_semaphore data_show_sem;//游动控制线程信号量

static Command s_datatype; //数据类型
static uint8_t s_dataaxis; //数据轴向(X, Y, Z)

//设置数据类型和数据轴向接口函数
void set_data_type(Command command, uint8_t data)
{
    s_datatype = command;
    s_dataaxis = data;
}

/*--------------------------------传感器数据更新线程-------------------------------*/
float timer_os_tick2;
//定时器释放信号量进入线程
void data_show_thread_tmrcallback(void* parameter)
{
    rt_tick_t ti;
    ti = rt_tick_get();
    timer_os_tick2 = (float)(ti)/(float)(RT_TICK_PER_SECOND);
    rt_sem_release(&data_show_sem);
    //rt_thread_resume(&data_show_thread);
}

//数据展示线程
static void data_show_thread_entry(void* parameter)
{
    //待发送数据缓存
    uint8_t databuf[12];
    //主体循环
    while(1)
    {
        if(rt_sem_take(&data_show_sem, RT_WAITING_FOREVER) ==  RT_EOK)//判断信号量是否可以使用
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
            //打包按协议发送数据
            RFLink_sendStruct(FRIEND_ID1, s_datatype, databuf, 4);
            rt_thread_delay(50);
        }
        rt_thread_delay(5);
    }
}

//创建数据展示线程
void data_show_thread_create(void)
{
    rt_err_t result_app;

    result_app = rt_thread_init(&data_show_thread,
                                 "data show thread",
                                  data_show_thread_entry,
                                  RT_NULL,
                                 (rt_uint8_t*)&data_show_thread_stack[0],
                                  sizeof(data_show_thread_stack),
                                                                25,50);//这个Tick没啥用，主要还得看rt_thread_delay

    rt_timer_init(&data_show_timer,
                                 "_data_show_timer",
                                  data_show_thread_tmrcallback,
                                  RT_NULL,
                              DATA_SHOW_PERIOD,  //unit:ms
                                  RT_TIMER_FLAG_PERIODIC);//周期性定时器
    rt_sem_init(&data_show_sem, "data_show_sem", 0, RT_IPC_FLAG_FIFO);//初始化一个信号量
  if(result_app == RT_EOK)
    {
        rt_thread_startup(&data_show_thread);//启动线程
    }
//  rt_timer_start(&data_show_timer);//启动定时器(后面关闭发送不需要停止定时器，至于停止定时器的用处之后再考虑)
}

//暂停运行接口函数
void data_show_app_stop(void)
{
    rt_timer_stop(&data_show_timer);
}

//恢复运行接口函数
void data_show_app_resume(void)
{
    rt_timer_start(&data_show_timer);
}

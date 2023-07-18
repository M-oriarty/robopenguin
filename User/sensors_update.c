#include "sensors_update.h"

#define SENSORS_THREAD_PRIORITY         25
#define SENSORS_THREAD_STACK_SIZE       1024

//#define TIME_CHECK


//#define PRINT_DATA

//static rt_sem_t sensors_update_sem;

/*　使用静态线程时，线程的栈需要设置字节对齐　*/
ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t sensors_thread_stack[SENSORS_THREAD_STACK_SIZE];
static struct rt_thread sensors_thread;

pd power_data;
float ori_angle;

int compareValue;

static void sensors_entry(void* parameter)
{
            static rt_tick_t now_os_tick;//当前获取的系统tick
            static rt_tick_t last_os_tick;

            float depth;
          float ori_depth; //初始深度

            //HAL_UART_Receive_IT(&huart2,Rx_data,50);

            ms5837_init();


//          get_pump_angle();
            //printf("ori_pump_angle %f\n",pump_ori_angle);

//          get_mass_angle();
            //printf("ori_mass_angle %f\n",mass_ori_angle);

            ori_depth = get_ms5837_data();

            while(1){


                    //时间戳
                    now_os_tick = rt_tick_get();
                    robopenguin.timestamp = (float)now_os_tick/(float)(RT_TICK_PER_SECOND);//unit:s

                    //限位开关
                    robopenguin.limit_state.pump_limit_state = limit_detect(PUMP_LIMIT);
                    robopenguin.limit_state.mass_limit_state = limit_detect(MASS_LIMIT);


//                  //磁编码器
//                  get_pump_final_angle(clockwise);
//                  robopenguin.pump_data.pump_angle = pump_final_angle;
//                  robopenguin.pump_data.pump_count = pump_count;
                SensorReadnByte_pump(0x80,(unsigned char *)&length_aveval_pump, 0x5E,2);
                pump_distance = (R_PUMP[0] * 16 + R_PUMP[1])/16/4;
                robopenguin.pump_distance = pump_distance;

//              SensorReadnByte_mass(0x80,(unsigned char *)&length_aveval_mass, 0x5E,2);
//              mass_distance = (R_mass[0] * 16 + R_mass[1])/16/4;
//              robopenguin.mass_distance = mass_distance;

//
//                  get_mass_final_angle(clockwise);
//                  robopenguin.mass_data.mass_angle = mass_final_angle;
//                  robopenguin.mass_data.mass_count = mass_count;


                    #ifdef TIME_CHECK
                    now_os_tick = rt_tick_get();
                    last_os_tick = now_os_tick;
                    #endif

                  //深度数据
                    depth = get_ms5837_data();
                    robopenguin.depth = depth-ori_depth;

                    #ifdef TIME_CHECK
                    now_os_tick = rt_tick_get();
                    printf("depth_time %f\n",(float)(now_os_tick-last_os_tick)/(float)(RT_TICK_PER_SECOND));
                    #endif

                    #ifdef TIME_CHECK
                    now_os_tick = rt_tick_get();
                    last_os_tick = now_os_tick;
                    #endif
                    //IMU数据
                    robopenguin.imu_data.accelx = JY901_data.acc.acc[0];
                    robopenguin.imu_data.accely = JY901_data.acc.acc[1];
                    robopenguin.imu_data.accelz = JY901_data.acc.acc[2];
                    robopenguin.imu_data.gyrox = JY901_data.gyro.gyro[0];
                    robopenguin.imu_data.gyroy = JY901_data.gyro.gyro[1];
                    robopenguin.imu_data.gyroz = JY901_data.gyro.gyro[2];
                    robopenguin.imu_data.roll = JY901_data.angle.angle[0];
                    robopenguin.imu_data.yaw = JY901_data.angle.angle[1];
                    robopenguin.imu_data.pitch = JY901_data.angle.angle[2];
                    #ifdef TIME_CHECK
                    now_os_tick = rt_tick_get();
                    printf("imu_time %f\n",(float)(now_os_tick-last_os_tick)/(float)(RT_TICK_PER_SECOND));
                    #endif

                    #ifdef TIME_CHECK
                    now_os_tick = rt_tick_get();
                    last_os_tick = now_os_tick;
                    #endif
                    //功率数据
                    //read_data();
                    //power_data = get_power_data();
                    read_data_pect();
                    read_data_glide();

                    robopenguin.power_data.Current_data[0] = (float)Current_data_pect/10000;
                    robopenguin.power_data.Voltage_data[0] = (float)Voltage_data_pect/10000;
                    robopenguin.power_data.Power_data[0] = (float)Power_data_pect/10000;

                    robopenguin.power_data.Current_data[1] = (float)Current_data_glide/10000;
                    robopenguin.power_data.Voltage_data[1] = (float)Voltage_data_glide/10000;
                    robopenguin.power_data.Power_data[1] = (float)Power_data_glide/10000;

//                  robopenguin.power_data.Energy_data = (float)power_data.Energy_data/10000;
//                  robopenguin.power_data.Pf_data = (float)power_data.Pf_data/10000;
//                  robopenguin.power_data.CO2_data = (float)power_data.CO2_data/10000;
                    #ifdef TIME_CHECK
                    now_os_tick = rt_tick_get();
                    printf("power_time %f\n",(float)(now_os_tick-last_os_tick)/(float)(RT_TICK_PER_SECOND));
                    #endif


                    #ifdef PRINT_DATA
                    printf("\n*****************data******************\n");
                    printf("timestamp %f\n",robopenguin.timestamp);

                    printf("limit %d %d\n",robopenguin.limit_state.pump_limit_state,robopenguin.limit_state.mass_limit_state);

                    printf("final depth %f\n",robopenguin.depth);

                    printf("angle:%f %f %f\n",robopenguin.imu_data.roll,robopenguin.imu_data.yaw,robopenguin.imu_data.pitch);

                    printf("power_data:%f %f %f\n",robopenguin.power_data.Current_data,robopenguin.power_data.Voltage_data,robopenguin.power_data.Power_data);

                    printf("pump_angle %f count %d\n",robopenguin.pump_data.pump_angle,robopenguin.pump_data.pump_count);

                    printf("mass_angle %f count %d\n",robopenguin.mass_data.mass_angle,robopenguin.mass_data.mass_count);

                    if(robopenguin.limit_state.pump_limit_state == 1){
                        printf("ok\n");
                    }else{
                        printf("not fine\n");
                    }

                    printf("***************************************\n");
                    #endif

//                  if(robopenguin.pump_distance >=3 && robopenguin.pump_distance <= 30){
//                      WK_PortSendData(1,"pump ok\n",8);
//                  }
//
//                  if(robopenguin.mass_distance >=3 && robopenguin.mass_distance <=30){
//                      WK_PortSendData(1,"mass ok\n",8);
//                  }
                    //WK_PortSendData(1,"hello\n",6);
                    //printf("power_data:%f %f %f\n",robopenguin.power_data.Current_data,robopenguin.power_data.Voltage_data,robopenguin.power_data.Power_data);

                    rt_thread_mdelay(5);
            }
}

void sensors_thread_create(void)
{

    rt_thread_init(&sensors_thread,
                                    "_SENSORS_UPDATE",
                                    sensors_entry,
                                    RT_NULL,
                                    sensors_thread_stack,
                                    SENSORS_THREAD_STACK_SIZE,
                                    SENSORS_THREAD_PRIORITY,30);

    rt_thread_startup(&sensors_thread);

}

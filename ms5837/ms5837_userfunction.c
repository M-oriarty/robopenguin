#include "MS5837_userfunction.h"

//#define TIME_CHECK

/*
C1 ѹ�������� SENS|T1
C2  ѹ������  OFF|T1
C3  �¶�ѹ��������ϵ�� TCS
C4  �¶�ϵ����ѹ������ TCO
C5  �ο��¶� T|REF
C6  �¶�ϵ�����¶� TEMPSENS
*/

//#define _MS5837_DEBUG

uint16_t  Cal_C[7];      // ���ڴ��PROM�е�6������1-6
int32_t dT,MS5837_TEMP;  // ȫ�ֱ���
float global_depth;

// ����д��
unsigned long MS5837_getConversion(uint8_t command)
{
    static rt_tick_t now_os_tick;//��ǰ��ȡ��ϵͳtick
    static rt_tick_t last_os_tick;

        unsigned long conversion = 0;
        uint8_t temp[3];

        IIC_Start();
        IIC_Send_Byte(0xEC);        //д��ַ
    #ifdef _MS5837_DEBUG
        printf("send EC\n");
    #endif

        #ifdef TIME_CHECK
        now_os_tick = rt_tick_get();
        last_os_tick = now_os_tick;
        #endif
        IIC_Wait_Ack();
        #ifdef TIME_CHECK
        now_os_tick = rt_tick_get();
        printf("wait_ack_time %f\n",(float)(now_os_tick-last_os_tick)/(float)(RT_TICK_PER_SECOND));
        #endif

        #ifdef TIME_CHECK
        now_os_tick = rt_tick_get();
        last_os_tick = now_os_tick;
        #endif
        IIC_Send_Byte(command); //дת������
        #ifdef TIME_CHECK
        now_os_tick = rt_tick_get();
        printf("send_byte_time %f\n",(float)(now_os_tick-last_os_tick)/(float)(RT_TICK_PER_SECOND));
        #endif

    #ifdef _MS5837_DEBUG
        printf("send %d\n",command);
    #endif

        #ifdef TIME_CHECK
        now_os_tick = rt_tick_get();
        last_os_tick = now_os_tick;
        #endif
        Delayms(10);   //����ת�������һ��Ҫ�ȴ�ת����ɣ�����
        #ifdef TIME_CHECK
        now_os_tick = rt_tick_get();
        printf("delay_10ms_time %f\n",(float)(now_os_tick-last_os_tick)/(float)(RT_TICK_PER_SECOND));
        #endif

        IIC_Wait_Ack();
        IIC_Stop();

        Delayms(10);
        IIC_Start();
        IIC_Send_Byte(0xEC);        //д��ַ
    #ifdef _MS5837_DEBUG
        printf("send EC\n");
    #endif
        IIC_Wait_Ack();
        IIC_Send_Byte(0);               // start read sequence
    #ifdef _MS5837_DEBUG
        printf("send 0 start read sequence\n");
    #endif
        IIC_Wait_Ack();
        IIC_Stop();


        IIC_Start();
        IIC_Send_Byte(0xEC+0x01);  //�������ģʽ
    #ifdef _MS5837_DEBUG
        printf("send EC + 01\n");
    #endif
        IIC_Wait_Ack();

        #ifdef TIME_CHECK
        now_os_tick = rt_tick_get();
        last_os_tick = now_os_tick;
        #endif
        temp[0] = IIC_Read_Byte(1);  //��ACK�Ķ�����  bit 23-16
        #ifdef TIME_CHECK
        now_os_tick = rt_tick_get();
        printf("read_byte_time %f\n",(float)(now_os_tick-last_os_tick)/(float)(RT_TICK_PER_SECOND));
        #endif

    #ifdef _MS5837_DEBUG
        printf("temp[0] %d\n",temp[0]);
    #endif
        temp[1] = IIC_Read_Byte(1);  //��ACK�Ķ�����  bit 8-15
    #ifdef _MS5837_DEBUG
        printf("temp[1] %d\n",temp[1]);
    #endif
        temp[2] = IIC_Read_Byte(0);  //��NACK�Ķ����� bit 0-7
    #ifdef _MS5837_DEBUG
        printf("temp[2] %d\n",temp[2]);
    #endif
        IIC_Stop();

        conversion = (unsigned long)temp[0] * 65536 + (unsigned long)temp[1] * 256 + (unsigned long)temp[2];
    #ifdef _MS5837_DEBUG
        printf("conversion %lu\n",conversion);
    #endif
        return conversion;
}


// ����д��
void MS5837_getTemperature(void)
{
    static rt_tick_t now_os_tick;//��ǰ��ȡ��ϵͳtick
    static rt_tick_t last_os_tick;

    #ifdef _MS5837_DEBUG
    printf("get temperature begin\n");
    #endif
    uint32_t D2_Temp;
    #ifdef _MS5837_DEBUG
    printf("get D2_Temp begin\n");
    #endif

    #ifdef TIME_CHECK
    now_os_tick = rt_tick_get();
    last_os_tick = now_os_tick;
    #endif
    D2_Temp = MS5837_getConversion(0x58);
    #ifdef TIME_CHECK
    now_os_tick = rt_tick_get();
    printf("get_temperature_conversion_time %f\n",(float)(now_os_tick-last_os_tick)/(float)(RT_TICK_PER_SECOND));
    #endif

    #ifdef _MS5837_DEBUG
    printf("get D2_Temp end\n");
    #endif
    dT=D2_Temp - (((int32_t)Cal_C[5])*256);
    #ifdef _MS5837_DEBUG
    printf("dT %d\n",dT);
    #endif
    MS5837_TEMP=2000+(int64_t)dT*((int64_t)Cal_C[6])/8388608;
    #ifdef _MS5837_DEBUG
    printf("MS5837_TEMP %d\n",MS5837_TEMP);
    printf("get temperature end\n");
    #endif
}

// ����д��
double MS5837_getPressure(void)
{
    static rt_tick_t now_os_tick;//��ǰ��ȡ��ϵͳtick
    static rt_tick_t last_os_tick;

    #ifdef _MS5837_DEBUG
    printf("get pressure begin\n");
    #endif

    double Pressure;

    uint32_t D1_Pres;

    int64_t OFF_; // OFF ʵ���¶Ȳ���
    int64_t SENS; // SENS ʵ���¶�������
    int64_t OFF2,SENS2;
    int32_t T2; //�¶�У��ֵ

    #ifdef TIME_CHECK
    now_os_tick = rt_tick_get();
    last_os_tick = now_os_tick;
    #endif
    D1_Pres= MS5837_getConversion(0x48);
    #ifdef TIME_CHECK
    now_os_tick = rt_tick_get();
    printf("get_pressure_conversion_time %f\n",(float)(now_os_tick-last_os_tick)/(float)(RT_TICK_PER_SECOND));
    #endif

    #ifdef _MS5837_DEBUG
    printf("D1_Pres %d\n",D1_Pres);
    #endif

    OFF_=(int64_t)Cal_C[2]*65536+((int64_t)Cal_C[4]*(int64_t)dT)/128;
    SENS=(int64_t)Cal_C[1]*32768+((int64_t)Cal_C[3]*(int64_t)dT)/256;

    #ifdef _MS5837_DEBUG
    printf("OFF_ %lld\n",OFF_);
    printf("SENS %lld\n",SENS);
    #endif

    //MS5837_TEMP = 2000;
    if(MS5837_TEMP<2000)  // low temp
    {
        T2 = 3*((int64_t)dT*(int64_t)dT) / 8589934592;  //MS5837
        OFF2 = 3 * (MS5837_TEMP - 2000) * (MS5837_TEMP - 2000)/2;
        SENS2 = 5 * (MS5837_TEMP - 2000) * (MS5837_TEMP - 2000)/8;
    }
    else
    {
        T2=2*((int64_t)dT*(int64_t)dT)/137438953472;//MS5837
        OFF2 = (MS5837_TEMP - 2000) * (MS5837_TEMP - 2000)/16;//MS5837
        SENS2 = 0;//MS5837
     }

    #ifdef _MS5837_DEBUG
    printf("dT %d\n",dT);
    printf("MS5837_TEMP %d\n",MS5837_TEMP);
    printf("T2 %d\n",T2);
    printf("OFF2 %lld\n",OFF2);
    printf("SENS2 %lld\n",SENS2);
    #endif

    OFF_ = OFF_ - OFF2;
    SENS = SENS - SENS2;

    #ifdef _MS5837_DEBUG
    printf("OFF_ %lld\n",OFF_);
    printf("SENS %lld\n",SENS);
    #endif

    Pressure=(D1_Pres*SENS/2097152-OFF_)/8192;

    MS5837_TEMP=(MS5837_TEMP-T2)/100;
    Pressure = (double)Pressure*10;//��λ����   pressure��10(����datasheet)��mbar����1000��bar��*100000��pa��=1��pa�����������10��ĵ�λ��Pa����˹��

    #ifdef _MS5837_DEBUG
    printf("Pressure %f Pa\n",Pressure);
    #endif

    #ifdef _MS5837_DEBUG
    printf("get pressure end\n");
    #endif
    return Pressure;
}


// **************************************************************
// �����豸
void ms5837_reset(void)
{
    #ifdef _MS5837_DEBUG
    printf("ms5837 reset begin\n");
    #endif
    Delayus(100);
    IIC_Start();
    IIC_Send_Byte(0xEC);//CSB�ӵأ�������ַ��0XEE������ 0X77
    #ifdef _MS5837_DEBUG
    printf("send EC\n");
    #endif
    IIC_Wait_Ack();
    IIC_Send_Byte(0x1E);//���͸�λ����
    #ifdef _MS5837_DEBUG
    printf("send 1E\n");
    #endif
    IIC_Wait_Ack();
    IIC_Stop();
    #ifdef _MS5837_DEBUG
    printf("ms5837 reset end\n");
    #endif
}
// �Լ��ĺ�������ʼ��������
void ms5837_init(void)
{
    uint8_t inth,intl;
    int i;
    ms5837_reset();
    Delayms(120);
    #ifdef _MS5837_DEBUG
    printf("ms5837 init begin\n");
    #endif
  for (i=1;i<=6;i++)
    {
        IIC_Start();
    IIC_Send_Byte(0xEC);
        #ifdef _MS5837_DEBUG
        printf("send EC\n");
        #endif
        IIC_Wait_Ack();
        IIC_Send_Byte(0xA0 + (i*2));
        #ifdef _MS5837_DEBUG
        printf("send A0 + %d\n",i*2);
        #endif
        IIC_Wait_Ack();
    IIC_Stop();
        Delayus(5);
        IIC_Start();
        IIC_Send_Byte(0xEC+0x01);  //�������ģʽ
        #ifdef _MS5837_DEBUG
        printf("send EC + 01\n");
        #endif
        Delayus(1);
        IIC_Wait_Ack();
        inth = IIC_Read_Byte(1);        //��ACK�Ķ�����
        #ifdef _MS5837_DEBUG
        printf("inth %d\n",inth);
        #endif
        Delayus(1);
        intl = IIC_Read_Byte(0);            //���һ���ֽ�NACK
        #ifdef _MS5837_DEBUG
        printf("intl %d\n",inth);
        #endif
        IIC_Stop();
    Cal_C[i] = (((uint16_t)inth << 8) | intl);
    }
    #ifdef _MS5837_DEBUG
    for (i=1;i<=6;i++){
        printf("Cal_C[%d] %d\n",i, Cal_C[i]);
    }
    printf("ms5837 init end\n");
    #endif
}

// ��ȡѹ������
float get_ms5837_data(void)
{
    static rt_tick_t now_os_tick;//��ǰ��ȡ��ϵͳtick
    static rt_tick_t last_os_tick;

    #ifdef _MS5837_DEBUG
    printf("get data begin\n");
    #endif
    double pressure;
    double ms5837_value;
    float depth = 0.0f;

    #ifdef TIME_CHECK
    now_os_tick = rt_tick_get();
    last_os_tick = now_os_tick;
    #endif
    MS5837_getTemperature();//��ȡ�¶�
    #ifdef TIME_CHECK
    now_os_tick = rt_tick_get();
    printf("get_temperature_time %f\n",(float)(now_os_tick-last_os_tick)/(float)(RT_TICK_PER_SECOND));
    #endif

    #ifdef TIME_CHECK
    now_os_tick = rt_tick_get();
    last_os_tick = now_os_tick;
    #endif
    pressure = MS5837_getPressure();   //��ȡѹ��,��λPa
    #ifdef TIME_CHECK
    now_os_tick = rt_tick_get();
    printf("get_pressure_time %f\n",(float)(now_os_tick-last_os_tick)/(float)(RT_TICK_PER_SECOND));
    #endif

    ms5837_value = pressure/100; // ��λ�ǣ�mbar��1 mbar=100 Pa
    //printf("pressure is %f\n",ms5837_value);
    // ����ѹ=101.325 Kpa=101325 Pa
    // P = rho*g*h
    // h = P/(rho*g)
    depth = 100*(ms5837_value*100 - 101300)/(1029*9.80665); // ��λ�����ף�ע������λ����������
    //printf("depth is %f cm\n",depth);
    global_depth = depth;
    #ifdef _MS5837_DEBUG
    printf("get data end\n");
    #endif
    return depth;
//  return pressure;
}


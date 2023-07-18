#include "MS5837.h"

#include "rtthread.h"
//#define TIME_CHECK

//#define _MS5837_DEBUG

/********************************************************************************
*
* Function name ��IIC_Start
* Description   ������IIC��ʼ�ź�
*
* Parameter     ��
*       @null
* Return        ��null
********************************************************************************/

void IIC_Start(void)
{
    SDAOUT;     //sda�����
    SDA_HIGH;
    SCL_HIGH;
    Delayus(4);
    SDA_LOW;//START:when CLK is high,DATA change form high to low
    Delayus(4);
    SCL_LOW;//ǯסI2C���ߣ�׼�����ͻ��������
    #ifdef _MS5837_DEBUG
    printf("iic start\n");
    #endif
}

/********************************************************************************
*
* Function name ��IIC_Stop
* Description   ������IICֹͣ�ź�
*
* Parameter     ��
*       @null
* Return        ��null
********************************************************************************/
void IIC_Stop(void)
{
    SDAOUT;//sda�����
    SDA_LOW;//STOP:when CLK is high DATA change form low to high
    SCL_HIGH;
    Delayus(4);
    SDA_HIGH;
    Delayus(4);
    #ifdef _MS5837_DEBUG
    printf("iic stop\n");
    #endif
}

/********************************************************************************
*
* Function name ��IIC_Wait_Ack
* Description   ���ȴ�Ӧ���źŵ���
*
* Parameter     ��
*       @null
* Return        ��1������Ӧ��ʧ�ܣ� 0������Ӧ��ɹ�
********************************************************************************/
uint8_t IIC_Wait_Ack(void)
{
    uint8_t count=0;
    SDAIN;

    SCL_HIGH;//�ͷ�����
    Delayus(1);
    SDA_HIGH; //����SDA
    Delayus(1);

    while(SDA) //��ȡSDA״̬��0ΪӦ��
    {
        count++;
        if(count>250)
        {
            IIC_Stop();
            return 1;
        }
    }
    SCL_LOW;
//  ACK++;    //��¼оƬ����Ӧ�ô�����������
    #ifdef _MS5837_DEBUG
    printf("count %d\n",count);
    #endif
    return 0;
}

/********************************************************************************
*
* Function name ��IIC_Ack
* Description   ������ACKӦ��
*
* Parameter     ��
*       @null
* Return        ��null
********************************************************************************/
void IIC_Ack(void)
{
    SDAOUT;
    SCL_LOW;
    SDA_LOW;
    Delayus(2);
    SCL_HIGH;
    Delayus(2);
    SCL_LOW;
}

/********************************************************************************
*
* Function name ��IIC_NAck
* Description   ��������ACKӦ��
*
* Parameter     ��
*       @null
* Return        ��null
********************************************************************************/
void IIC_NAck(void)
{
    SDAOUT;
    SCL_LOW;
    SDA_HIGH;
    Delayus(2);
    SCL_HIGH;
    Delayus(2);
    SCL_LOW;
}

/********************************************************************************
*
* Function name ��IIC_Send_Byte
* Description   ��IIC����һ���ֽ�
*
* Parameter     ��
*       @null
* Return        �����شӻ�����Ӧ��1����Ӧ��0����Ӧ��
********************************************************************************/
void IIC_Send_Byte(uint8_t txd)
{
    static rt_tick_t now_os_tick;//��ǰ��ȡ��ϵͳtick
    static rt_tick_t last_os_tick;

    uint8_t i;
    SDAOUT;
    SCL_LOW;         //scl=0,�������ݱ仯

    #ifdef TIME_CHECK
    now_os_tick = rt_tick_get();
    last_os_tick = now_os_tick;
    #endif
    Delayus(4);
    #ifdef TIME_CHECK
    now_os_tick = rt_tick_get();
    printf("delay_4us_time %f\n",(float)(now_os_tick-last_os_tick)/(float)(RT_TICK_PER_SECOND));
    #endif

    for(i=0;i<8;i++)
    {
        if(txd&0x80)SDA_HIGH;
        else SDA_LOW;
        txd<<=1;
        Delayus(4);

        SCL_HIGH;    //SCL=1,���ݱ���
        Delayus(4);

        SCL_LOW;
        Delayus(4);
    }
}

/********************************************************************************
*
* Function name ��IIC_Read_Byte
* Description   ����1���ֽ�
*
* Parameter     ��
*       @ack      ��ack=1ʱ������ACK��ack=0������nACK��
* Return        ��null
********************************************************************************/
uint8_t IIC_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
    SDAIN;//SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
    {
        SCL_LOW;//IIC_SCL=0;
        Delayus(2);
            SCL_HIGH;//IIC_SCL=1;
        receive<<=1;
        if(SDA)receive++;   //if(READ_SDA)receive++;
        Delayus(1);
    }
    if (!ack)
        IIC_NAck();//IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK
    return receive;
}

void SDA_OUT(void){
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    //GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void SDA_IN(void){
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStruct. GPIO_Mode = GPIO_Mode_IPU;
    //GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
}

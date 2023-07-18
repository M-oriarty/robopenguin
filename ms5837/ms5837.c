#include "MS5837.h"

#include "rtthread.h"
//#define TIME_CHECK

//#define _MS5837_DEBUG

/********************************************************************************
*
* Function name ：IIC_Start
* Description   ：产生IIC起始信号
*
* Parameter     ：
*       @null
* Return        ：null
********************************************************************************/

void IIC_Start(void)
{
    SDAOUT;     //sda线输出
    SDA_HIGH;
    SCL_HIGH;
    Delayus(4);
    SDA_LOW;//START:when CLK is high,DATA change form high to low
    Delayus(4);
    SCL_LOW;//钳住I2C总线，准备发送或接收数据
    #ifdef _MS5837_DEBUG
    printf("iic start\n");
    #endif
}

/********************************************************************************
*
* Function name ：IIC_Stop
* Description   ：产生IIC停止信号
*
* Parameter     ：
*       @null
* Return        ：null
********************************************************************************/
void IIC_Stop(void)
{
    SDAOUT;//sda线输出
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
* Function name ：IIC_Wait_Ack
* Description   ：等待应答信号到来
*
* Parameter     ：
*       @null
* Return        ：1，接收应答失败； 0，接收应答成功
********************************************************************************/
uint8_t IIC_Wait_Ack(void)
{
    uint8_t count=0;
    SDAIN;

    SCL_HIGH;//释放总线
    Delayus(1);
    SDA_HIGH; //拉高SDA
    Delayus(1);

    while(SDA) //读取SDA状态，0为应答
    {
        count++;
        if(count>250)
        {
            IIC_Stop();
            return 1;
        }
    }
    SCL_LOW;
//  ACK++;    //记录芯片给的应该次数，调试用
    #ifdef _MS5837_DEBUG
    printf("count %d\n",count);
    #endif
    return 0;
}

/********************************************************************************
*
* Function name ：IIC_Ack
* Description   ：产生ACK应答
*
* Parameter     ：
*       @null
* Return        ：null
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
* Function name ：IIC_NAck
* Description   ：不产生ACK应答
*
* Parameter     ：
*       @null
* Return        ：null
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
* Function name ：IIC_Send_Byte
* Description   ：IIC发送一个字节
*
* Parameter     ：
*       @null
* Return        ：返回从机有无应答：1，有应答；0，无应答。
********************************************************************************/
void IIC_Send_Byte(uint8_t txd)
{
    static rt_tick_t now_os_tick;//当前获取的系统tick
    static rt_tick_t last_os_tick;

    uint8_t i;
    SDAOUT;
    SCL_LOW;         //scl=0,允许数据变化

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

        SCL_HIGH;    //SCL=1,数据保持
        Delayus(4);

        SCL_LOW;
        Delayus(4);
    }
}

/********************************************************************************
*
* Function name ：IIC_Read_Byte
* Description   ：读1个字节
*
* Parameter     ：
*       @ack      ：ack=1时，发送ACK；ack=0，发送nACK。
* Return        ：null
********************************************************************************/
uint8_t IIC_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
    SDAIN;//SDA_IN();//SDA设置为输入
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
        IIC_NAck();//IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK
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

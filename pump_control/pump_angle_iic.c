#include "pump_angle_iic.h"

//#define _PUMP_DEBUG

void PUMP_IIC_Start(void)
{
    PUMP_SDAOUT;     //sda线输出
    PUMP_SDA_HIGH;
    Delayus(1);
    PUMP_SCL_HIGH;
    Delayus(4);
    PUMP_SDA_LOW;//START:when CLK is high,DATA change form high to low
    Delayus(4);
    PUMP_SCL_LOW;//钳住I2C总线，准备发送或接收数据
    #ifdef _PUMP_DEBUG
    printf("iic start\n");
    #endif
}

void PUMP_IIC_Stop(void)
{
    PUMP_SCL_LOW;
    PUMP_SDAOUT;//sda线输出
    PUMP_SDA_LOW;//STOP:when CLK is high DATA change form low to high
    PUMP_SCL_HIGH;
    Delayus(4);
    PUMP_SDA_HIGH;
    Delayus(4);
    #ifdef _PUMP_DEBUG
    printf("iic stop\n");
    #endif
}

uint8_t PUMP_IIC_Wait_Ack(void)
{
    #ifdef _PUMP_DEBUG
    printf("iic wait ack begin\n");
    #endif
    uint8_t count=0;
    PUMP_SDAIN;

    PUMP_SCL_HIGH;//释放总线
    Delayus(1);
    PUMP_SDA_HIGH; //拉高SDA
    Delayus(1);

    while(PUMP_SDA) //读取SDA状态，0为应答
    {
        count++;
        if(count>250)
        {
            PUMP_IIC_Stop();
            return 1;
        }
    }
    #ifdef _PUMP_DEBUG
    printf("PUMP_SDA %d\n",PUMP_SDA);
    printf("count %d\n",count);
    #endif
    Delayus(1);
    PUMP_SCL_LOW;
//  ACK++;    //记录芯片给的应该次数，调试用
    PUMP_SDAOUT;
    PUMP_SDA_LOW;
    Delayus(1);
    #ifdef _PUMP_DEBUG
    printf("iic wait ack end\n");
    #endif
    return 0;
}

void PUMP_IIC_Ack(void)
{
    PUMP_SCL_LOW;
    PUMP_SDAOUT;
    PUMP_SDA_LOW;
    Delayus(2);
    PUMP_SCL_HIGH;
    Delayus(2);
    PUMP_SCL_LOW;
    #ifdef _PUMP_DEBUG
    printf("iic ack\n");
    #endif
}

void PUMP_IIC_NAck(void)
{
    PUMP_SCL_LOW;
    PUMP_SDAOUT;
    PUMP_SDA_HIGH;
    Delayus(2);
    PUMP_SCL_HIGH;
    Delayus(2);
    PUMP_SCL_LOW;
    #ifdef _PUMP_DEBUG

    printf("iic nack\n");
    #endif
}

void PUMP_IIC_Send_Byte(uint8_t txd)
{
    #ifdef _PUMP_DEBUG
    printf("iic send byte begin\n");
    #endif
    uint8_t i;
    PUMP_SDAOUT;
    PUMP_SCL_LOW;         //scl=0,允许数据变化

    Delayus(10);

    for(i=0;i<8;i++)
    {
        if(txd&0x80)PUMP_SDA_HIGH;
        else PUMP_SDA_LOW;
        txd<<=1;
        Delayus(4);

        PUMP_SCL_HIGH;    //SCL=1,数据保持
        Delayus(4);

        PUMP_SCL_LOW;
        Delayus(4);
    }
    #ifdef _PUMP_DEBUG
    printf("iic send byte end\n");
    #endif
}

uint8_t PUMP_IIC_Read_Byte(unsigned char ack)
{
    #ifdef _PUMP_DEBUG
    printf("iic read byte\n");
    #endif
    unsigned char i,receive=0;
    PUMP_SDAIN;//SDA_IN();//SDA设置为输入
    PUMP_SDA_HIGH;
    PUMP_SCL_LOW;
    Delayus(20);
    for(i=0;i<8;i++ )
    {
        PUMP_SCL_LOW;//IIC_SCL=0;
        Delayus(2);
            PUMP_SCL_HIGH;//IIC_SCL=1;
        receive<<=1;
        if(PUMP_SDA)receive++;   //if(READ_SDA)receive++;
        Delayus(1);
    }
    if (!ack)
        PUMP_IIC_NAck();//IIC_NAck();//发送nACK
    else
        PUMP_IIC_Ack(); //发送ACK
    return receive;
}

void PUMP_SDA_OUT(void){
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    //GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void PUMP_SDA_IN(void){
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStruct. GPIO_Mode = GPIO_Mode_IPU;
    //GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
}

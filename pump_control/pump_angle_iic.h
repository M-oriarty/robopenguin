#ifndef _PUMP_ANGLE_IIC_H
#define _PUMP_ANGLE_IIC_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "ch32v30x.h"
#include "ch32v30x_gpio.h"
#include "delay.h"

//IO设置
#define PUMP_SCL_HIGH     GPIO_SetBits(GPIOB,GPIO_Pin_0)//GPIO_SetBits(GPIOB,GPIO_PIN_6) /////PA4
#define PUMP_SCL_LOW      GPIO_ResetBits(GPIOB,GPIO_Pin_0)//GPIO_ResetBits(GPIOB,GPIO_PIN_6)  /////PA5
#define PUMP_SDA_HIGH     GPIO_SetBits(GPIOB,GPIO_Pin_1)//GPIO_SetBits(GPIOB,GPIO_PIN_7)
#define PUMP_SDA_LOW      GPIO_ResetBits(GPIOB,GPIO_Pin_1)//GPIO_ResetBits(GPIOB,GPIO_PIN_7)
#define PUMP_SDA          GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)//SDA_READ
#define PUMP_SDAOUT       PUMP_SDA_OUT()//GPIOB->MODER=(GPIOB->MODER&0XFFFFFFFF)|(0X00000001<<(2*11));  //复用推挽输出
#define PUMP_SDAIN        PUMP_SDA_IN()//GPIOB->MODER=(GPIOB->MODER&0XFFFFFFFF)&(~(0X00000003<<(2*11)));  //浮空输入

//IIC所有操作函数
void PUMP_SDA_OUT(void);
void PUMP_SDA_IN(void);
void PUMP_IIC_Start(void);              //发送IIC开始信号
void PUMP_IIC_Stop(void);               //发送IIC停止信号
void PUMP_IIC_Send_Byte(uint8_t txd);           //IIC发送一个字节
uint8_t PUMP_IIC_Read_Byte(unsigned char ack);//IIC2读取一个字节
uint8_t PUMP_IIC_Wait_Ack(void);                //IIC等待ACK信号
void PUMP_IIC_Ack(void);                    //IIC发送ACK信号
void PUMP_IIC_NAck(void);               //IIC不发送ACK信号

#endif

#ifndef __MS5837_H
#define __MS5837_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "ch32v30x.h"
#include "delay.h"


/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室，正点原子
File name:
Description:
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History:
*****************************************************************************/

//IO设置
#define SCL_HIGH     GPIO_SetBits(GPIOC,GPIO_Pin_4)//GPIO_SetBits(GPIOB,GPIO_PIN_6)
#define SCL_LOW      GPIO_ResetBits(GPIOC,GPIO_Pin_4)//GPIO_ResetBits(GPIOB,GPIO_PIN_6)
#define SDA_HIGH     GPIO_SetBits(GPIOC,GPIO_Pin_5)//GPIO_SetBits(GPIOB,GPIO_PIN_7)
#define SDA_LOW      GPIO_ResetBits(GPIOC,GPIO_Pin_5)//GPIO_ResetBits(GPIOB,GPIO_PIN_7)
#define SDA          GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)//SDA_READ
#define SDAOUT       SDA_OUT()//GPIOB->MODER=(GPIOB->MODER&0XFFFFFFFF)|(0X00000001<<(2*11));  //复用推挽输出
#define SDAIN        SDA_IN()//GPIOB->MODER=(GPIOB->MODER&0XFFFFFFFF)&(~(0X00000003<<(2*11)));  //浮空输入

//IIC所有操作函数
void SDA_OUT(void);
void SDA_IN(void);
void IIC_Start(void);               //发送IIC开始信号
void IIC_Stop(void);                //发送IIC停止信号
void IIC_Send_Byte(uint8_t txd);            //IIC发送一个字节
uint8_t IIC_Read_Byte(unsigned char ack);//IIC2读取一个字节
uint8_t IIC_Wait_Ack(void);                 //IIC等待ACK信号
void IIC_Ack(void);                 //IIC发送ACK信号
void IIC_NAck(void);                //IIC不发送ACK信号
#endif

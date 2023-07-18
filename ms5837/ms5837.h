#ifndef __MS5837_H
#define __MS5837_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "ch32v30x.h"
#include "delay.h"


/*****************************************************************************
Copyright: CASIA ����������ʵ���ң�����ԭ��
File name:
Description:
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History:
*****************************************************************************/

//IO����
#define SCL_HIGH     GPIO_SetBits(GPIOC,GPIO_Pin_4)//GPIO_SetBits(GPIOB,GPIO_PIN_6)
#define SCL_LOW      GPIO_ResetBits(GPIOC,GPIO_Pin_4)//GPIO_ResetBits(GPIOB,GPIO_PIN_6)
#define SDA_HIGH     GPIO_SetBits(GPIOC,GPIO_Pin_5)//GPIO_SetBits(GPIOB,GPIO_PIN_7)
#define SDA_LOW      GPIO_ResetBits(GPIOC,GPIO_Pin_5)//GPIO_ResetBits(GPIOB,GPIO_PIN_7)
#define SDA          GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)//SDA_READ
#define SDAOUT       SDA_OUT()//GPIOB->MODER=(GPIOB->MODER&0XFFFFFFFF)|(0X00000001<<(2*11));  //�����������
#define SDAIN        SDA_IN()//GPIOB->MODER=(GPIOB->MODER&0XFFFFFFFF)&(~(0X00000003<<(2*11)));  //��������

//IIC���в�������
void SDA_OUT(void);
void SDA_IN(void);
void IIC_Start(void);               //����IIC��ʼ�ź�
void IIC_Stop(void);                //����IICֹͣ�ź�
void IIC_Send_Byte(uint8_t txd);            //IIC����һ���ֽ�
uint8_t IIC_Read_Byte(unsigned char ack);//IIC2��ȡһ���ֽ�
uint8_t IIC_Wait_Ack(void);                 //IIC�ȴ�ACK�ź�
void IIC_Ack(void);                 //IIC����ACK�ź�
void IIC_NAck(void);                //IIC������ACK�ź�
#endif

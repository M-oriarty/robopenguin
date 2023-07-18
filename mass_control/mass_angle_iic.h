#ifndef _MASS_ANGLE_IIC_H
#define _MASS_ANGLE_IIC_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "ch32v30x.h"
#include "ch32v30x_gpio.h"
#include "delay.h"

//IO����
#define MASS_SCL_HIGH     GPIO_SetBits(GPIOB,GPIO_Pin_10)//GPIO_SetBits(GPIOB,GPIO_PIN_6)
#define MASS_SCL_LOW      GPIO_ResetBits(GPIOB,GPIO_Pin_10)//GPIO_ResetBits(GPIOB,GPIO_PIN_6)
#define MASS_SDA_HIGH     GPIO_SetBits(GPIOB,GPIO_Pin_11)//GPIO_SetBits(GPIOB,GPIO_PIN_7)
#define MASS_SDA_LOW      GPIO_ResetBits(GPIOB,GPIO_Pin_11)//GPIO_ResetBits(GPIOB,GPIO_PIN_7)
#define MASS_SDA          GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)//SDA_READ
#define MASS_SDAOUT       MASS_SDA_OUT()//GPIOB->MODER=(GPIOB->MODER&0XFFFFFFFF)|(0X00000001<<(2*11));  //�����������
#define MASS_SDAIN        MASS_SDA_IN()//GPIOB->MODER=(GPIOB->MODER&0XFFFFFFFF)&(~(0X00000003<<(2*11)));  //��������

//IIC���в�������
void MASS_SDA_OUT(void);
void MASS_SDA_IN(void);
void MASS_IIC_Start(void);              //����IIC��ʼ�ź�
void MASS_IIC_Stop(void);               //����IICֹͣ�ź�
void MASS_IIC_Send_Byte(uint8_t txd);           //IIC����һ���ֽ�
uint8_t MASS_IIC_Read_Byte(unsigned char ack);//IIC2��ȡһ���ֽ�
uint8_t MASS_IIC_Wait_Ack(void);                //IIC�ȴ�ACK�ź�
void MASS_IIC_Ack(void);                    //IIC����ACK�ź�
void MASS_IIC_NAck(void);               //IIC������ACK�ź�

#endif

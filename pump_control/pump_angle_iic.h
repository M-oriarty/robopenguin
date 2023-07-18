#ifndef _PUMP_ANGLE_IIC_H
#define _PUMP_ANGLE_IIC_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "ch32v30x.h"
#include "ch32v30x_gpio.h"
#include "delay.h"

//IO����
#define PUMP_SCL_HIGH     GPIO_SetBits(GPIOB,GPIO_Pin_0)//GPIO_SetBits(GPIOB,GPIO_PIN_6) /////PA4
#define PUMP_SCL_LOW      GPIO_ResetBits(GPIOB,GPIO_Pin_0)//GPIO_ResetBits(GPIOB,GPIO_PIN_6)  /////PA5
#define PUMP_SDA_HIGH     GPIO_SetBits(GPIOB,GPIO_Pin_1)//GPIO_SetBits(GPIOB,GPIO_PIN_7)
#define PUMP_SDA_LOW      GPIO_ResetBits(GPIOB,GPIO_Pin_1)//GPIO_ResetBits(GPIOB,GPIO_PIN_7)
#define PUMP_SDA          GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)//SDA_READ
#define PUMP_SDAOUT       PUMP_SDA_OUT()//GPIOB->MODER=(GPIOB->MODER&0XFFFFFFFF)|(0X00000001<<(2*11));  //�����������
#define PUMP_SDAIN        PUMP_SDA_IN()//GPIOB->MODER=(GPIOB->MODER&0XFFFFFFFF)&(~(0X00000003<<(2*11)));  //��������

//IIC���в�������
void PUMP_SDA_OUT(void);
void PUMP_SDA_IN(void);
void PUMP_IIC_Start(void);              //����IIC��ʼ�ź�
void PUMP_IIC_Stop(void);               //����IICֹͣ�ź�
void PUMP_IIC_Send_Byte(uint8_t txd);           //IIC����һ���ֽ�
uint8_t PUMP_IIC_Read_Byte(unsigned char ack);//IIC2��ȡһ���ֽ�
uint8_t PUMP_IIC_Wait_Ack(void);                //IIC�ȴ�ACK�ź�
void PUMP_IIC_Ack(void);                    //IIC����ACK�ź�
void PUMP_IIC_NAck(void);               //IIC������ACK�ź�

#endif

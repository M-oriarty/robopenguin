#ifndef __SPI_H
#define __SPI_H

#include <rtdevice.h>
#include <stdlib.h>
#include <rthw.h>
#include <rtthread.h>
#include "ch32v30x.h"

void SPI_GPIO_Init(void);            //��ʼ��SPI��
void SPI_SetSpeed(u8 SpeedSet); //����SPI�ٶ�
u8 SPI_ReadWriteByte(u8 TxData);//SPI���߶�дһ���ֽ�
void SPI_CS_H(void);
void SPI_CS_L(void);
void rt_hw_spi_init(void);

#endif

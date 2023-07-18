#ifndef __SPI_H
#define __SPI_H

#include <rtdevice.h>
#include <stdlib.h>
#include <rthw.h>
#include <rtthread.h>
#include "ch32v30x.h"

void SPI_GPIO_Init(void);            //初始化SPI口
void SPI_SetSpeed(u8 SpeedSet); //设置SPI速度
u8 SPI_ReadWriteByte(u8 TxData);//SPI总线读写一个字节
void SPI_CS_H(void);
void SPI_CS_L(void);
void rt_hw_spi_init(void);

#endif

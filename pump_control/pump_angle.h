#ifndef _PUMP_ANGLE_H
#define _PUMP_ANGLE_H

#include <stdbool.h>
#include "delay.h"
#include "pump_angle_iic.h"
#include "usart.h"
#include <stdio.h>
#include "sensors_update.h"
#include "rtthread.h"

/***************´Å±àÂëÆ÷*****************/
//#define PUMP_SLAVE_ADDR  0x0C
//#define PUMP_MEM1_ADDR   0x03
//#define PUMP_MEM2_ADDR   0x04

//#define clockwise 0  //Ë³Ê±Õë
//#define counterclockwise 1 //ÄæÊ±Õë

//float get_PUMP_data(void);
//void get_pump_angle(void);
//void get_pump_final_angle(int command);
/****************************************/

extern unsigned char R_PUMP[2];

unsigned char SensorWritenByte_pump(unsigned char Devid, unsigned char *TXBuff, unsigned char SubAdd, unsigned char Size);
unsigned char SensorReadnByte_pump(unsigned char Devid, unsigned char *RXBuff, unsigned char SubAdd, unsigned char Size);

void UserI2c_Init(void);

unsigned char SensorReadByte_pump(unsigned char Devid, unsigned char *RXBuff, unsigned char SubAdd, unsigned char Size);

void ScanI2CDevices_pump();

#endif

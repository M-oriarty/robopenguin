#ifndef _MASS_ANGLE_H
#define _MASS_ANGLE_H

#include <stdbool.h>
#include "delay.h"
#include "mass_angle_iic.h"
#include "usart.h"
#include <stdio.h>
#include "sensors_update.h"
#include "rtthread.h"

/***************´Å±àÂëÆ÷*****************/
//#define MASS_SLAVE_ADDR  0x0C
//#define MASS_MEM1_ADDR   0x03
//#define MASS_MEM2_ADDR   0x04

//#define clockwise 0  //Ë³Ê±Õë
//#define counterclockwise 1 //ÄæÊ±Õë

//extern float mass_ori_angle;
//extern float mass_last_angle;
//extern float mass_final_angle;
//extern float mass_count;

//float get_MASS_data(void);
//void get_mass_angle(void);
//void get_mass_final_angle(int command);
/****************************************/

extern int shift;
extern unsigned char R_mass[2];

unsigned char SensorWritenByte_mass(unsigned char Devid, unsigned char *TXBuff, unsigned char SubAdd, unsigned char Size);
unsigned char SensorReadnByte_mass(unsigned char Devid, unsigned char *RXBuff, unsigned char SubAdd, unsigned char Size);

void UserI2c_Init(void);

unsigned char SensorReadByte_mass(unsigned char Devid, unsigned char *RXBuff, unsigned char SubAdd, unsigned char Size);

void ScanI2CDevices_mass();


#endif

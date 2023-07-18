#ifndef _IM1253B_H
#define _IM1253B_H

#include "usart.h"
#include "wk2124.h"

extern unsigned char read_enable_pect,receive_finished_pect,reveive_number_pect;
extern unsigned char read_enable_glide,receive_finished_glide,reveive_number_glide;

extern unsigned char UART6_Rx_Buffer[40];

extern unsigned char Rx_Buffer_pect[40];
extern unsigned char Rx_Buffer_glide[40];

extern unsigned long Voltage_data_pect,Current_data_pect,Power_data_pect;
extern unsigned long Voltage_data_glide,Current_data_glide,Power_data_glide;

unsigned int calccrc(unsigned char crcbuf,unsigned int crc);
unsigned int chkcrc(unsigned char *buf,unsigned char len);
void read_data_pect(void);
void read_data_glide(void);
void Analysis_data_pect(void);
void Analysis_data_glide(void);
#endif

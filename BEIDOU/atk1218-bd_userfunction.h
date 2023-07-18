#ifndef __ATK1218_BD_H
#define __ATK1218_BD_H

#include <rtthread.h>
#include <ch32v30x.h>
#include <stdbool.h>
#include "stdint.h"
#include "BEIDOU.h"

#define BEIDOU_MAX_MESSAGE_LEN 150

__IO uint8_t bd_uartBuf[BEIDOU_MAX_MESSAGE_LEN];


void BD_get_position(uint32_t *longitude_data, uint32_t *latitude_data, uint8_t *ewhemi, uint8_t *nshemi);//¾­¶È£¬Î³¶È
void beidou_init(void);
void bd_measurementAnalysis(void);

#endif


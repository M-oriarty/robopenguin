/*
 * JY_901_userfunction.h
 *
 *  Created on: 2023年5月7日
 *      Author: 27220
 */

#ifndef JY_901_JY_901_USERFUNCTION_H_
#define JY_901_JY_901_USERFUNCTION_H_

#include "JY_901.h"
#include "stdio.h"
#include "string.h"
#include "usart.h"

#define RXBUFFER_LEN 33

typedef struct
{
    float angle[3];
}Angle;

typedef struct
{
    float acc[3];
}Acc;

typedef struct
{
    float gyro[3];
}SGyro;

typedef struct JY_901_Buffer
{
    uint8_t Rx_flag;                                //接收完成标志
    uint8_t Rx_len;                                 //接收长度
    uint8_t frame_head;                             //帧头
    uint8_t RxBuffer[RXBUFFER_LEN];                 //数据存储
    Angle angle;
    Acc acc;
    SGyro gyro;
}JY_901_Buffer;


extern JY_901_Buffer JY901_data;

void JY901_Process(void);
void JY_901_Buffer_Init(JY_901_Buffer *Data);

#endif /* JY_901_JY_901_USERFUNCTION_H_ */

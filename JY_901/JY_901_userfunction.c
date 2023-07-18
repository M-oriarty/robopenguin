/*
 * JY_901_userfunction.c
 *
 *  Created on: 2023年5月7日
 *      Author: 27220
 */

#include "JY_901_userfunction.h"

JY_901_Buffer JY901_data;

struct SAcc         stcAcc;
struct SGyro        stcGyro;
struct SAngle   stcAngle;

//初始化函数
void JY_901_Buffer_Init(JY_901_Buffer *Data)
{
        for(uint16_t i=0; i < RXBUFFER_LEN; i++)    Data->RxBuffer[i] = 0;
        Data->frame_head = 0x55;
        Data->Rx_flag = 0;
        Data->Rx_len = 0;
        printf("frame_head: %d  flag: %d  len: %d\n",Data->frame_head,Data->Rx_flag,Data->Rx_len);
}

void JY901_Process()
{
        for(uint8_t i=0;i<4;i++)
        {
                if(JY901_data.RxBuffer[i*11]!= JY901_data.frame_head) continue;
                switch(JY901_data.RxBuffer[i*11+1])
                {
                        case 0x51:
                            memcpy(&stcAcc,&JY901_data.RxBuffer[2 + i*11],8);
                            for(uint8_t j = 0; j < 3; j++) JY901_data.acc.acc[j] = (float)stcAcc.a[j]/32768*16;                                 //加速度
                        break;
                        case 0x52:
                            memcpy(&stcGyro,&JY901_data.RxBuffer[2 + i*11],8);
                            for(uint8_t j = 0; j < 3; j++) JY901_data.gyro.gyro[j] = (float)stcGyro.w[j]/32768*2000;                                //角速度
                        break;
                        case 0x53:
                            memcpy(&stcAngle,&JY901_data.RxBuffer[2 + i*11],8);
                            for(uint8_t j = 0; j < 3; j++) JY901_data.angle.angle[j] = (float)stcAngle.Angle[j]/32768*180;      //角度
                        break;
                }

        }

}

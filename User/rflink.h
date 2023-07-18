#ifndef _RFLINK_H
#define _RFLINK_H
#include "string.h"
#include "usart.h"

#include "wk2124.h"

#define HOST_ID 0x11 // ��λ��
#define SLAVE_ID 0x22 // ���ص���
#define LOCAL_ID 0x33 // ��������

#define MY_ID      LOCAL_ID
#define FRIEND_ID1 HOST_ID  //��λ��
#define FRIEND_ID2 SLAVE_ID //���ص���

typedef struct RFMessage
{
    unsigned char sender_id;
    unsigned char receiver_id;
    unsigned char length;
    unsigned char data[120];//���ɿ��ܲ�����
}RFMessage;

typedef enum Recstate
{
    WAITING_FF=0,
    SENDER_ID,
    RECEIVER_ID,
    RECEIVE_LEN,
    RECEIVE_PACKAGE,
    RECEIVE_CHECK
}Recstate;

typedef enum Command  //����������ͨ��ָ���
{
    SHAKING_HANDS=1,
    SYNCHRONIZE_CLOCK,
    SET_SWIM_RUN,
    SET_SWIM_START,
    SET_SWIM_STOP,
    SET_SWIM_FORCESTOP,
    SET_LPECT_FLAP,
  SET_LPECT_FEATHER,
    SET_LPECT_PITCH,
    SET_RPECT_FLAP,
    SET_RPECT_FEATHER,
    SET_RPECT_PITCH,
    SET_DATASHOW_OVER,
    SET_TAIL,
    SET_PUMP_OFF,
    SET_PUMP_IN,
    SET_PUMP_OUT,
    SET_LONGITUDINAL_MASS_FMOVE,  //�¼ӵ�ǰ���ƶ����򻬿�
    SET_LONGITUDINAL_MASS_BMOVE,  //�¼ӵĺ����ƶ����򻬿�
    SET_LONGITUDINAL_MASS_STOP,//�¼ӵ����򻬿�ֹͣ
    READ_ROBOT_STATUS,
    READ_ROBOT_DATA,
    READ_IMU_ATTITUDE,
  READ_IMU_ACCEL,
    READ_IMU_GYRO,
    READ_DEPTH,
    READ_CURRENT,
  READ_VOLTAGE,
  READ_POWER,
  READ_FLAP,
    READ_FEATHER,
    READ_PITCH,
    READ_TAIL
}Command;

typedef struct AnalysisResult
{
    Command command;
    unsigned char message[32];
    uint8_t message_len;
}AnalysisResult;

uint8_t RFLink_byteAnalysisCall(uint8_t *rx_data);
uint8_t RFLink_receiveStates(uint8_t *rx_data);
void RFLinkPrintf ( const RFMessage* tx_message );
AnalysisResult RFLink_commandAnalysis(void);
void RFLink_sendStruct(uint8_t receiver_id, const Command command_type , unsigned char* p , uint8_t len);

#endif

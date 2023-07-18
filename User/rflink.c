#include "rflink.h"

#include <rtthread.h>
#include "rflink.h"
#include "stdarg.h"
#include <stdbool.h>

static uint32_t s_u32_analysis_package_count = 0;
static uint32_t s_u32_receive_message_count = 0;
static uint8_t s_u8_shaking_hands_state = 0;

static RFMessage tx_message;
static RFMessage rx_message_set;
static uint32_t receive_check_sum_set;
static uint8_t receive_message_length_set;
static uint16_t byte_count_set;

uint8_t RFLink_byteAnalysisCall(uint8_t *rx_data)
{
    if(RFLink_receiveStates(rx_data))
    {
        return 1;//�ɹ�����1
    }
    return 0;
}


uint8_t RFLink_receiveStates(uint8_t *rx_data)//һ���ֽ�һ���ֽڵĶ�ȡ���ݲ�ִ����Ӧ�������ԱȺ�У�ԣ�ע��rx_message_set��������ݲ�����������
{
    uint32_t receive_check_sum = receive_check_sum_set;
    uint8_t receive_message_length = receive_message_length_set;
    uint16_t byte_count = byte_count_set;
    int i = 0;

    if(rx_data[i] == 0xff){
        receive_check_sum =0;
        receive_message_length = 0;
        byte_count = 0;
        receive_check_sum += rx_data[i++];
        if(rx_data[i] == FRIEND_ID1){
            rx_message_set.sender_id = rx_data[i];
            receive_check_sum += rx_data[i++];
        }
        if(rx_data[i] == MY_ID){
            rx_message_set.receiver_id = rx_data[i];
            receive_check_sum += rx_data[i++];
        }
        receive_message_length = rx_data[i];
        rx_message_set.length = receive_message_length;
        receive_check_sum += rx_data[i++];
        for(byte_count=0; byte_count<=receive_message_length; byte_count++){
            rx_message_set.data[byte_count] = rx_data[i];
            receive_check_sum += rx_data[i++];
        }
        receive_check_sum=receive_check_sum % 255;
        if((uint8_t)receive_check_sum == rx_data[i])
            receive_check_sum=0;
            return 1 ;
    }

    return 0;
}

AnalysisResult RFLink_commandAnalysis()//�������յ������ݣ�������ָ�������(���������һ��'\0'��û���յ������ݲ���Ϊ0xff)������
{
    AnalysisResult result;

    result.command = (Command)rx_message_set.data[0];//������ת������Ҳ����
    result.message_len = rx_message_set.length;//���Ȳ�����ָ��

    if(s_u8_shaking_hands_state==0 && result.command != SHAKING_HANDS && result.command != SYNCHRONIZE_CLOCK) //if not  shaking hands
    {
        // ��λ��û�к���λ�����ֵĻ������е�ָ�Ϊ0
        result.command = 0;
        return result;
    }

    if(result.command == SHAKING_HANDS || result.command == SYNCHRONIZE_CLOCK)
    {
        s_u8_shaking_hands_state = 1;
    }

    if(rx_message_set.length != 0)
    {
        memcpy(result.message, rx_message_set.data+1,rx_message_set.length);//���﷢��ָ��û�к��ڳ��ȵ���
        *(result.message+rx_message_set.length) = '\0';//��message�����һ��'\0'�������RFLink_message()�����зǳ�����
    }
    else
    {
        result.message[0] = 0xFF;//û���յ�����Ϊ0xFF
    }
    return result;
}

void RFLink_sendStruct(uint8_t receiver_id, const Command command_type , unsigned char* p , uint8_t len)//���lenֻ��*p�ĳ���
{
    tx_message.sender_id = MY_ID;
    tx_message.receiver_id = receiver_id;
    tx_message.length=len;//����ָ��command_type��Ҫ�����1
    tx_message.data[0] = (unsigned char)command_type;
    if(len > 0)
    {
        memcpy(&tx_message.data[1], p, len);
    }

    if(receiver_id==FRIEND_ID1||receiver_id==FRIEND_ID2)RFLinkPrintf(&tx_message);
}

uint16_t len;
void RFLinkPrintf ( const RFMessage* tx_message ){
        uint32_t send_check_sum=0;
      uint8_t  answer=0;
      uint32_t i = 0;
      rt_uint8_t sendbuf[120];
      sendbuf[0] = 0xff;
      send_check_sum += sendbuf[0];
      sendbuf[1] = tx_message->sender_id;
      send_check_sum += sendbuf[1];
      sendbuf[2] = tx_message->receiver_id;
      send_check_sum += sendbuf[2];
      sendbuf[3] = tx_message->length;//����sender_id��receiver_id��length����command�Լ�У����֮��ĳ����ܺͣ�����ʵ�ʴ��������ݵĳ���
      send_check_sum += sendbuf[3];
      memcpy(&sendbuf[4],&tx_message->data[0],(tx_message->length+1));//+1��Ϊ�˼���ָ����һλ
      for (i=0;i<=sendbuf[3];i++)//+1��Ϊ�˼���ָ����һλ
      {
            send_check_sum += sendbuf[4+i];//�ӵ�4λ��ʼ�ӵģ���Ϊ��4λ��ʼ��������
        }
        answer = send_check_sum % 255;
      memcpy(&sendbuf[5+(tx_message->length)],&answer,1);
        USART_SendData(USART3, sendbuf);
        len = (tx_message->length)+6;
        WK_PortSendData(1,sendbuf,len);
        rt_thread_delay(20);
}

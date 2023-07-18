#include "IM1253B.h"

//模块默认 ID 为 0x01，波特率为 4800bps，数据格式为 8， N， 1

#define Read_ID 0x01

unsigned char read_enable_pect,receive_finished_pect,reveive_number_pect;
unsigned char read_enable_glide,receive_finished_glide,reveive_number_glide;

//unsigned char UART6_Rx_Buffer[40];

unsigned char Tx_Buffer_pect[8];
unsigned char Rx_Buffer_pect[40];

unsigned char Tx_Buffer_glide[8];
unsigned char Rx_Buffer_glide[40];

unsigned long Voltage_data_pect,Current_data_pect,Power_data_pect;
unsigned long Voltage_data_glide,Current_data_glide,Power_data_glide;

unsigned int calccrc(unsigned char crcbuf,unsigned int crc)
{
    unsigned char i;
    unsigned char chk;
    crc=crc ^ crcbuf;
    for(i=0;i<8;i++)
    {
        chk=( unsigned char)(crc&1);
        crc=crc>>1;
        crc=crc&0x7fff;
        if (chk==1)
        crc=crc^0xa001;
        crc=crc&0xffff;
    }
    return crc;
}

unsigned int chkcrc(unsigned char *buf,unsigned char len)
{
    unsigned char hi,lo;
    unsigned int i;
    unsigned int crc;
    crc=0xFFFF;
    for(i=0;i<len;i++)
    {
        crc=calccrc(*buf,crc);
        buf++;
    }
    hi=( unsigned char)(crc%256);
    lo=( unsigned char)(crc/256);
    crc=(((unsigned int)(hi))<<8)|lo;
    return crc;
}

void read_data_pect(void)
{
    union crcdata
    {
        unsigned int word16;
        unsigned char byte[2];
    }crcnow;
        read_enable_pect=0;
        Tx_Buffer_pect[0]=Read_ID; //模块的 ID 号，默认 ID 为 0x01
        Tx_Buffer_pect[1]=0x03;
        Tx_Buffer_pect[2]=0x00;
        Tx_Buffer_pect[3]=0x48;
        Tx_Buffer_pect[4]=0x00;
        Tx_Buffer_pect[5]=0x06;
        crcnow.word16=chkcrc(Tx_Buffer_pect,6);
        Tx_Buffer_pect[6]=crcnow.byte[1]; //CRC 效验低字节在前
        Tx_Buffer_pect[7]=crcnow.byte[0];

        USART_SendData(UART6, Tx_Buffer_pect);//发送 8 个数据
        WK_PortSendData(1,"read_pect\n",10);
}

void read_data_glide(void)
{
    union crcdata
    {
        unsigned int word16;
        unsigned char byte[2];
    }crcnow;
        read_enable_glide=0;
        Tx_Buffer_glide[0]=Read_ID; //模块的 ID 号，默认 ID 为 0x01
        Tx_Buffer_glide[1]=0x03;
        Tx_Buffer_glide[2]=0x00;
        Tx_Buffer_glide[3]=0x48;
        Tx_Buffer_glide[4]=0x00;
        Tx_Buffer_glide[5]=0x06;
        crcnow.word16=chkcrc(Tx_Buffer_glide,6);
        Tx_Buffer_glide[6]=crcnow.byte[1]; //CRC 效验低字节在前
        Tx_Buffer_glide[7]=crcnow.byte[0];
        USART_SendData(USART3,Tx_Buffer_glide);//发送 8 个数据
}

int send_pect;
void Analysis_data_pect(void)
{
    union crcdata
    {
        unsigned int word16;
        unsigned char byte[2];
    }crcnow;
    WK_PortSendData(1,"analysis\n",9);
    if(Rx_Buffer_pect[0]==Read_ID){
        WK_PortSendData(1,"id right\n",9);
    }

    if(Rx_Buffer_pect[0]==Read_ID) //确认 ID 正确
    {
        crcnow.word16=chkcrc(Rx_Buffer_pect,reveive_number_pect-2); //reveive_number 是接收数据总长度
        if((crcnow.byte[0]==Rx_Buffer_pect[reveive_number_pect-1])&&(crcnow.byte[1]==Rx_Buffer_pect[reveive_number_pect-2]))//确认 CRC 校验正确
        {
            Voltage_data_pect = (((unsigned long)(Rx_Buffer_pect[3]))<<24)|(((unsigned long)(Rx_Buffer_pect[4]))<<16)|(((unsigned long)(Rx_Buffer_pect[5]))<<8)|Rx_Buffer_pect[6];
            Current_data_pect = (((unsigned long)(Rx_Buffer_pect[7]))<<24)|(((unsigned long)(Rx_Buffer_pect[8]))<<16)|(((unsigned long)(Rx_Buffer_pect[9]))<<8)|Rx_Buffer_pect[10];
            Power_data_pect = (((unsigned long)(Rx_Buffer_pect[11]))<<24)|(((unsigned long)(Rx_Buffer_pect[12]))<<16)|(((unsigned long)(Rx_Buffer_pect[13]))<<8)|Rx_Buffer_pect[14];
//          Energy_data = (((unsigned long)(Rx_Buffer_pect[15]))<<24)|(((unsigned long)(Rx_Buffer_pect[16]))<<16)|(((unsigned long)(Rx_Buffer_pect[17]))<<8)|Rx_Buffer_pect[18];
//          Pf_data = (((unsigned long)(Rx_Buffer_pect[19]))<<24)|(((unsigned long)(Rx_Buffer_pect[20]))<<16)|(((unsigned long)(Rx_Buffer_pect[21]))<<8)|Rx_Buffer_pect[22];
//          CO2_data = (((unsigned long)(Rx_Buffer_pect[23]))<<24)|(((unsigned long)(Rx_Buffer_pect[24]))<<16)|(((unsigned long)(Rx_Buffer_pect[25]))<<8)|Rx_Buffer_pect[26];
        }
    }

}

void Analysis_data_glide(void)
{
    union crcdata
    {
        unsigned int word16;
        unsigned char byte[2];
    }crcnow;

    if(Rx_Buffer_glide[0]==Read_ID) //确认 ID 正确
    {
        crcnow.word16=chkcrc(Rx_Buffer_glide,reveive_number_glide-2); //reveive_number 是接收数据总长度
        if((crcnow.byte[0]==Rx_Buffer_glide[reveive_number_glide-1])&&(crcnow.byte[1]==Rx_Buffer_pect[reveive_number_glide-2]))//确认 CRC 校验正确
        {
            Voltage_data_glide = (((unsigned long)(Rx_Buffer_glide[3]))<<24)|(((unsigned long)(Rx_Buffer_glide[4]))<<16)|(((unsigned long)(Rx_Buffer_glide[5]))<<8)|Rx_Buffer_glide[6];
            Current_data_glide = (((unsigned long)(Rx_Buffer_glide[7]))<<24)|(((unsigned long)(Rx_Buffer_glide[8]))<<16)|(((unsigned long)(Rx_Buffer_glide[9]))<<8)|Rx_Buffer_glide[10];
            Power_data_glide = (((unsigned long)(Rx_Buffer_glide[11]))<<24)|(((unsigned long)(Rx_Buffer_glide[12]))<<16)|(((unsigned long)(Rx_Buffer_glide[13]))<<8)|Rx_Buffer_glide[14];
//          Energy_data = (((unsigned long)(Rx_Buffer_glide[15]))<<24)|(((unsigned long)(Rx_Buffer_glide[16]))<<16)|(((unsigned long)(Rx_Buffer_glide[17]))<<8)|Rx_Buffer_glide[18];
//          Pf_data = (((unsigned long)(Rx_Buffer_glide[19]))<<24)|(((unsigned long)(Rx_Buffer_glide[20]))<<16)|(((unsigned long)(Rx_Buffer_glide[21]))<<8)|Rx_Buffer_glide[22];
//          CO2_data = (((unsigned long)(Rx_Buffer_glide[23]))<<24)|(((unsigned long)(Rx_Buffer_glide[24]))<<16)|(((unsigned long)(Rx_Buffer_glide[25]))<<8)|Rx_Buffer_glide[26];
        }
    }

}

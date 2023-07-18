/*------------------------ʵ�ֺ���--------------------------
 *����
 *���ܣ�     ��ȡ���Ƕ�λ�ľ�γ��
 *���룺
 *���أ�     ��γ�ȵ��ַ���
 *--------------------------------------------------------*/
#include "atk1218-bd_userfunction.h"

nmea_msg gnssinfo;

//��������Ľ���
void bd_measurementAnalysis(void)
{
    GPS_Analysis(&gnssinfo,(u8*)bd_uartBuf);//�����ַ���
}

void BD_get_position(uint32_t *longitude_data, uint32_t *latitude_data, uint8_t *ewhemi, uint8_t *nshemi)//���ȣ�γ��
{
    uint32_t tp;
    uint8_t tp_hemi;

    //�й���
    if (gnssinfo.gpsvalue_validity == 'A')//����������Ч�Թ���
    {
        tp = gnssinfo.longitude;
        *longitude_data = tp;     //��ȡ��������
        tp = gnssinfo.latitude;
        *latitude_data = tp;      //��ȡγ������
    }

    tp_hemi = gnssinfo.ewhemi;
    *ewhemi = tp_hemi;         //��ȡ��|�����ַ�
    tp_hemi = gnssinfo.nshemi;
    *nshemi = tp_hemi;         //��ȡ��|��γ�ַ�

    //�޹���
//  tp = gnssinfo.longitude;
//  *longitude_data = tp;     //��ȡ��������
//  tp = gnssinfo.latitude;
//  *latitude_data = tp;      //��ȡγ������
//
//  tp_hemi = gnssinfo.ewhemi;
//  *ewhemi = tp_hemi;         //��ȡ��|�����ַ�
//  tp_hemi = gnssinfo.nshemi;
//  *nshemi = tp_hemi;         //��ȡ��|��γ�ַ�
}

//��ʼ���ӿں���
void beidou_init(void)
{
    bd_BuffPrintf_Init();//�ҵ����򿪱�����λ�豸
}

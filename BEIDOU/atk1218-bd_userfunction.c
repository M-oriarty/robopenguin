/*------------------------实现函数--------------------------
 *函数
 *功能：     读取卫星定位的经纬度
 *输入：
 *返回：     经纬度的字符串
 *--------------------------------------------------------*/
#include "atk1218-bd_userfunction.h"

nmea_msg gnssinfo;

//测量结果的解析
void bd_measurementAnalysis(void)
{
    GPS_Analysis(&gnssinfo,(u8*)bd_uartBuf);//分析字符串
}

void BD_get_position(uint32_t *longitude_data, uint32_t *latitude_data, uint8_t *ewhemi, uint8_t *nshemi)//经度，纬度
{
    uint32_t tp;
    uint8_t tp_hemi;

    //有过滤
    if (gnssinfo.gpsvalue_validity == 'A')//根据数据有效性过滤
    {
        tp = gnssinfo.longitude;
        *longitude_data = tp;     //读取经度数据
        tp = gnssinfo.latitude;
        *latitude_data = tp;      //读取纬度数据
    }

    tp_hemi = gnssinfo.ewhemi;
    *ewhemi = tp_hemi;         //读取东|西经字符
    tp_hemi = gnssinfo.nshemi;
    *nshemi = tp_hemi;         //读取南|北纬字符

    //无过滤
//  tp = gnssinfo.longitude;
//  *longitude_data = tp;     //读取经度数据
//  tp = gnssinfo.latitude;
//  *latitude_data = tp;      //读取纬度数据
//
//  tp_hemi = gnssinfo.ewhemi;
//  *ewhemi = tp_hemi;         //读取东|西经字符
//  tp_hemi = gnssinfo.nshemi;
//  *nshemi = tp_hemi;         //读取南|北纬字符
}

//初始化接口函数
void beidou_init(void)
{
    bd_BuffPrintf_Init();//找到并打开北斗定位设备
}

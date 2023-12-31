/*
 * BEIDOU.c
 * 北斗芯片数据解析接口
 * 日期: 2022.3.4
 * 作者: Sijie Li
 * 瑕疵在于：此处没有北斗芯片相关写入函数
 */
#include "BEIDOU.h"

const u32 BAUD_id[9] = {4800,9600,19200,38400,57600,115200,230400,460800,921600};

/*****************************************************************
 *@函数名：NMEA协议逗号位置获取函数
 *@功能描述：从函数输入参数buf中得到第cx个逗号所在的位置
 *@输入参数：u8 *buf，待搜索字符串；u8 cx，所需获取的第cx个逗号位置
 *@返回值：0~0XFE,代表逗号所在位置的偏移；0XFF，代表不存在第cx个逗号
 *****************************************************************/
//********这里*buf<''可能会出问题，有可能需要空格
u8 NMEA_Comma_Pos(u8 *buf, u8 cx){
    u8 *p=buf;
    while(cx){
        if(*buf=='*'||*buf<' '||*buf>'z')return 0XFF;//遇到'*'或非法字符
        if(*buf==',')cx--;
        buf++;
    }
    return buf-p;
}

/******************************************************************
 *@函数名：NMEA求取次方函数
 *@功能描述：m^{n}次方
 *@输入参数：u8 m，底数；u8 n，指数
 *@返回值：u32 result，m^{n}次方结果
 ******************************************************************/
u32 NMEA_Pow(u8 m,u8 n){
    u32 result = 1;
    while(n--)result*=m;
    return result;
}

/******************************************************************
 *@函数名：NMEA字符串转换为数字的函数
 *@功能描述：str转换为数字，以','或者'*'结束
 *@输入参数：u8 *buf,数字存储区；u8 *dx,小数点位数，返回给调用函数
 *@返回值：转换后的数值(不含小数点，dx代表了小数点后的位数，可进行转换)
 ******************************************************************/
int NMEA_Str2num(u8 *buf, u8 *dx){
    u8 *p=buf;
    u32 ires=0,fres=0;
    u8 ilen=0,flen=0,i;
    u8 mask=0;
    int res;
    while(1){//得到整数和小数的长度
        if(*p=='-'){mask|=0X02;p++;}//是负数
        if(*p==','||(*p=='*'))break;//遇到结束了
        if(*p=='.'){mask|=0X01;p++;}//遇到小数点了
        else if(*p>'9'||(*p<'0')){//有非法字符
            ilen=0;
            flen=0;
            break;
        }
        if(mask&0X01)flen++;
        else ilen++;
        p++;
    }
    if(mask&0X02)buf++;//去掉负号
    for(i=0;i<ilen;i++){//得到整数部分数据
        ires+=NMEA_Pow(10,ilen-1-i)*(buf[i]-'0');
    }
    if(flen>5)flen=5;//最多取5位小数
    *dx=flen;//小数点位数
    for(i=0;i<flen;i++){//得到小数部分数据
        fres+=NMEA_Pow(10,flen-1-i)*(buf[ilen+1+i]-'0');
    }
    res=ires*NMEA_Pow(10,flen)+fres;
    if(mask&0X02)res=-res;
    return res;
}

/******************************************************************
 *@函数名：NMEA分析GPGSV信息
 *@功能描述：分析GPGSV信息，采用gpsx:nmea信息结构体
 *@输入参数：nmea_msg *gpsx,nmea信息结构体; u8 *buf,接收到的GPS/北斗数据缓冲区首地址
 *@返回值：无
 ******************************************************************/
void NMEA_GPGSV_Analysis(nmea_msg *gpsx,u8 *buf){
    u8 *p,*p1,dx;
    u8 len,i,j,slx=0;
    u8 posx;
    p=buf;
    p1=(u8*)strstr((const char*)p,"$GPGSV");//????没见过strstr这个函数***
    len=p1[7]-'0';//得到GPGSV的条数
    posx=NMEA_Comma_Pos(p1,3);//得到可见卫星总数
    if(posx!=0XFF)gpsx->svnum=NMEA_Str2num(p1+posx,&dx);
    for(i=0;i<len;i++){
        p1=(u8*)strstr((const char*)p,"$GPGSV");//????没见过strstr这个函数***
        for(j=0;j<4;j++){
            posx=NMEA_Comma_Pos(p1,4+j*4);
            if(posx!=0XFF)gpsx->slmsg[slx].num=NMEA_Str2num(p1+posx,&dx);//得到卫星编号
            else break;
            posx=NMEA_Comma_Pos(p1,5+j*4);
            if(posx!=0XFF)gpsx->slmsg[slx].eledeg=NMEA_Str2num(p1+posx,&dx);//得到卫星仰角
            else break;
            posx=NMEA_Comma_Pos(p1,6+j*4);
            if(posx!=0XFF)gpsx->slmsg[slx].azideg=NMEA_Str2num(p1+posx,&dx);//得到卫星方位角
            else break;
            posx=NMEA_Comma_Pos(p1,7+j*4);
            if(posx!=0XFF)gpsx->slmsg[slx].sn=NMEA_Str2num(p1+posx,&dx);//得到卫星信噪比
            else break;
            slx++;
        }
        p=p1+1;//切换到下一个GPGSV信息
    }
}

/******************************************************************
 *@函数名：NMEA分析BDGSV
 *@功能描述：分析BDGSV信息，采用gpsx:nmea信息结构体
 *@输入参数：nmea_msg *gpsx,nmea信息结构体; u8 *buf,接收到的北斗数据缓冲区首地址
 *@返回值：无
 ******************************************************************/
void NMEA_BDGSV_Analysis(nmea_msg *gpsx, u8 *buf){
    u8 *p,*p1,dx;
    u8 len,i,j,slx=0;
    u8 posx;
    p=buf;
    p1=(u8*)strstr((const char*)p, "$BDGSV");
    len=p1[7]-'0';//得到BDGSV的条数
    posx=NMEA_Comma_Pos(p1,3);//得到可见北斗卫星总数
    if(posx!=0XFF)gpsx->beidou_svnum=NMEA_Str2num(p1+posx,&dx);
    for(i=0;i<len;i++){
        p1=(u8*)strstr((const char*)p,"$BDGSV");
        for(j=0;j<4;j++){
            posx=NMEA_Comma_Pos(p1,4+j*4);
            if(posx!=0XFF)gpsx->beidou_slmsg[slx].beidou_num=NMEA_Str2num(p1+posx,&dx); //得到卫星编号
            else break;
            posx=NMEA_Comma_Pos(p1,5+j*4);
            if(posx!=0XFF)gpsx->beidou_slmsg[slx].beidou_eledeg=NMEA_Str2num(p1+posx,&dx);//得到卫星仰角
            else break;
            posx=NMEA_Comma_Pos(p1,6+j*4);
            if(posx!=0XFF)gpsx->beidou_slmsg[slx].beidou_azideg=NMEA_Str2num(p1+posx,&dx);//得到卫星方位角
            else break;
            posx=NMEA_Comma_Pos(p1,7+j*4);
            if(posx!=0XFF)gpsx->beidou_slmsg[slx].beidou_sn=NMEA_Str2num(p1+posx,&dx);  //得到卫星信噪比
            else break;
            slx++;
        }
        p=p1+1;//切换到下一个BDGSV信息
    }
}

/******************************************************************
 *@函数名：NMEA分析GNGGA
 *@功能描述：分析GNGGA信息，采用gpsx:nmea信息结构体
 *@输入参数：nmea_msg *gpsx,nmea信息结构体; u8 *buf,接收到的GPS数据缓冲区首地址
 *@返回值：无
 ******************************************************************/
void NMEA_GNGGA_Analysis(nmea_msg *gpsx,u8 *buf){
    u8 *p1,dx;
    u8 posx;
    p1=(u8*)strstr((const char *)buf,"$GNGGA");
    posx=NMEA_Comma_Pos(p1,6);                              //得到GPS状态
    if(posx!=0XFF)gpsx->gpssta=NMEA_Str2num(p1+posx,&dx);
    posx=NMEA_Comma_Pos(p1,7);                              //得到用于定位的卫星数
    if(posx!=0XFF)gpsx->posslnum=NMEA_Str2num(p1+posx,&dx);
    posx=NMEA_Comma_Pos(p1,9);                              //得到海拔高度
    if(posx!=0XFF)gpsx->altitude=NMEA_Str2num(p1+posx,&dx);
}

/******************************************************************
 *@函数名：NMEA分析GNGSA
 *@功能描述：分析GNGSA信息，采用gpsx:nmea信息结构体
 *@输入参数：nmea_msg *gpsx,nmea信息结构体; u8 *buf,接收到的GPS数据缓冲区首地址
 *@返回值：无
 ******************************************************************/
void NMEA_GNGSA_Analysis(nmea_msg *gpsx,u8 *buf){
    u8 *p1,dx;
    u8 posx;
    u8 i;
    p1=(u8*)strstr((const char *)buf,"$GNGSA");
    posx=NMEA_Comma_Pos(p1,2);                              //得到定位类型
    if(posx!=0XFF)gpsx->fixmode=NMEA_Str2num(p1+posx,&dx);
    for(i=0;i<12;i++)                                       //得到定位卫星编号
    {
        posx=NMEA_Comma_Pos(p1,3+i);
        if(posx!=0XFF)gpsx->possl[i]=NMEA_Str2num(p1+posx,&dx);
        else break;
    }
    posx=NMEA_Comma_Pos(p1,15);                             //得到PDOP位置精度因子
    if(posx!=0XFF)gpsx->pdop=NMEA_Str2num(p1+posx,&dx);
    posx=NMEA_Comma_Pos(p1,16);                             //得到HDOP位置精度因子
    if(posx!=0XFF)gpsx->hdop=NMEA_Str2num(p1+posx,&dx);
    posx=NMEA_Comma_Pos(p1,17);                             //得到VDOP位置精度因子
    if(posx!=0XFF)gpsx->vdop=NMEA_Str2num(p1+posx,&dx);
}

/******************************************************************
 *@函数名：NMEA分析GNRMC
 *@功能描述：分析GNRMC信息，采用gpsx:nmea信息结构体
 *@输入参数：nmea_msg *gpsx,nmea信息结构体; u8 *buf,接收到的GPS数据缓冲区首地址
 *@返回值：无
 ******************************************************************/
void NMEA_GNRMC_Analysis(nmea_msg *gpsx,u8 *buf){
    u8 *p1,dx;
    u8 posx;
    u32 temp;
    float rs;
    p1=(u8*)strstr((const char *)buf,"$GNRMC");//"$GNRMC",经常有&和GNRMC分开的情况,故只判断GPRMC.
    posx=NMEA_Comma_Pos(p1,1);                              //得到UTC时间
    if(posx!=0XFF){
        temp=NMEA_Str2num(p1+posx,&dx)/NMEA_Pow(10,dx);     //得到UTC时间,去掉ms
        gpsx->utc.hour=temp/10000;
        gpsx->utc.min=(temp/100)%100;
        gpsx->utc.sec=temp%100;
    }
    posx=NMEA_Comma_Pos(p1,2);//得到定位数据的有效性
    if(posx!=0XFF)gpsx->gpsvalue_validity=*(p1+posx);
    posx=NMEA_Comma_Pos(p1,3);                              //得到纬度
    if(posx!=0XFF){
        temp=NMEA_Str2num(p1+posx,&dx);
        gpsx->latitude=temp/NMEA_Pow(10,dx+2);  //得到°
        rs=temp%NMEA_Pow(10,dx+2);              //得到'
        gpsx->latitude=gpsx->latitude*NMEA_Pow(10,5)+(rs*NMEA_Pow(10,5-dx))/60;//转换为°
    }
    posx=NMEA_Comma_Pos(p1,4);                              //南纬还是北纬
    if(posx!=0XFF)gpsx->nshemi=*(p1+posx);
    posx=NMEA_Comma_Pos(p1,5);                              //得到经度
    if(posx!=0XFF){
        temp=NMEA_Str2num(p1+posx,&dx);
        gpsx->longitude=temp/NMEA_Pow(10,dx+2); //得到°
        rs=temp%NMEA_Pow(10,dx+2);              //得到'
        gpsx->longitude=gpsx->longitude*NMEA_Pow(10,5)+(rs*NMEA_Pow(10,5-dx))/60;//转换为°
    }
    posx=NMEA_Comma_Pos(p1,6);                              //东经还是西经
    if(posx!=0XFF)gpsx->ewhemi=*(p1+posx);
    posx=NMEA_Comma_Pos(p1,9);                              //得到UTC日期
    if(posx!=0XFF){
        temp=NMEA_Str2num(p1+posx,&dx);                     //得到UTC日期
        gpsx->utc.date=temp/10000;
        gpsx->utc.month=(temp/100)%100;
        gpsx->utc.year=2000+temp%100;
    }
}

/******************************************************************
 *@函数名：NMEA分析GNVTG
 *@功能描述：分析GNVTG信息，采用gpsx:nmea信息结构体
 *@输入参数：nmea_msg *gpsx,nmea信息结构体; u8 *buf,接收到的GPS数据缓冲区首地址
 *@返回值：无
 ******************************************************************/
void NMEA_GNVTG_Analysis(nmea_msg *gpsx,u8 *buf){
    u8 *p1,dx;
    u8 posx;
    p1=(u8*)strstr((const char *)buf,"$GNVTG");
    posx=NMEA_Comma_Pos(p1,7);                              //得到地面速率
    if(posx!=0XFF){
        gpsx->speed=NMEA_Str2num(p1+posx,&dx);
        if(dx<3)gpsx->speed*=NMEA_Pow(10,3-dx);             //确保扩大1000倍
    }
}

/******************************************************************
 *@函数名：NMEA-0183
 *@功能描述：提取NMEA-0183，采用gpsx:nmea信息结构体
 *@输入参数：nmea_msg *gpsx,nmea信息结构体; u8 *buf,接收到的GPS数据缓冲区首地址
 *@返回值：无
 ******************************************************************/
void GPS_Analysis(nmea_msg *gpsx,u8 *buf)
{
//  NMEA_GPGSV_Analysis(gpsx,buf);  //GPGSV解析
//  NMEA_BDGSV_Analysis(gpsx,buf);  //BDGSV解析
//  NMEA_GNGGA_Analysis(gpsx,buf);  //GNGGA解析
//  NMEA_GNGSA_Analysis(gpsx,buf);  //GPNSA解析
    NMEA_GNRMC_Analysis(gpsx,buf);  //GPNMC解析
//  NMEA_GNVTG_Analysis(gpsx,buf);  //GPNTG解析
}



/*
 * BEIDOU.c
 * ����оƬ���ݽ����ӿ�
 * ����: 2022.3.4
 * ����: Sijie Li
 * 覴����ڣ��˴�û�б���оƬ���д�뺯��
 */
#include "BEIDOU.h"

const u32 BAUD_id[9] = {4800,9600,19200,38400,57600,115200,230400,460800,921600};

/*****************************************************************
 *@��������NMEAЭ�鶺��λ�û�ȡ����
 *@�����������Ӻ����������buf�еõ���cx���������ڵ�λ��
 *@���������u8 *buf���������ַ�����u8 cx�������ȡ�ĵ�cx������λ��
 *@����ֵ��0~0XFE,����������λ�õ�ƫ�ƣ�0XFF���������ڵ�cx������
 *****************************************************************/
//********����*buf<''���ܻ�����⣬�п�����Ҫ�ո�
u8 NMEA_Comma_Pos(u8 *buf, u8 cx){
    u8 *p=buf;
    while(cx){
        if(*buf=='*'||*buf<' '||*buf>'z')return 0XFF;//����'*'��Ƿ��ַ�
        if(*buf==',')cx--;
        buf++;
    }
    return buf-p;
}

/******************************************************************
 *@��������NMEA��ȡ�η�����
 *@����������m^{n}�η�
 *@���������u8 m��������u8 n��ָ��
 *@����ֵ��u32 result��m^{n}�η����
 ******************************************************************/
u32 NMEA_Pow(u8 m,u8 n){
    u32 result = 1;
    while(n--)result*=m;
    return result;
}

/******************************************************************
 *@��������NMEA�ַ���ת��Ϊ���ֵĺ���
 *@����������strת��Ϊ���֣���','����'*'����
 *@���������u8 *buf,���ִ洢����u8 *dx,С����λ�������ظ����ú���
 *@����ֵ��ת�������ֵ(����С���㣬dx������С������λ�����ɽ���ת��)
 ******************************************************************/
int NMEA_Str2num(u8 *buf, u8 *dx){
    u8 *p=buf;
    u32 ires=0,fres=0;
    u8 ilen=0,flen=0,i;
    u8 mask=0;
    int res;
    while(1){//�õ�������С���ĳ���
        if(*p=='-'){mask|=0X02;p++;}//�Ǹ���
        if(*p==','||(*p=='*'))break;//����������
        if(*p=='.'){mask|=0X01;p++;}//����С������
        else if(*p>'9'||(*p<'0')){//�зǷ��ַ�
            ilen=0;
            flen=0;
            break;
        }
        if(mask&0X01)flen++;
        else ilen++;
        p++;
    }
    if(mask&0X02)buf++;//ȥ������
    for(i=0;i<ilen;i++){//�õ�������������
        ires+=NMEA_Pow(10,ilen-1-i)*(buf[i]-'0');
    }
    if(flen>5)flen=5;//���ȡ5λС��
    *dx=flen;//С����λ��
    for(i=0;i<flen;i++){//�õ�С����������
        fres+=NMEA_Pow(10,flen-1-i)*(buf[ilen+1+i]-'0');
    }
    res=ires*NMEA_Pow(10,flen)+fres;
    if(mask&0X02)res=-res;
    return res;
}

/******************************************************************
 *@��������NMEA����GPGSV��Ϣ
 *@��������������GPGSV��Ϣ������gpsx:nmea��Ϣ�ṹ��
 *@���������nmea_msg *gpsx,nmea��Ϣ�ṹ��; u8 *buf,���յ���GPS/�������ݻ������׵�ַ
 *@����ֵ����
 ******************************************************************/
void NMEA_GPGSV_Analysis(nmea_msg *gpsx,u8 *buf){
    u8 *p,*p1,dx;
    u8 len,i,j,slx=0;
    u8 posx;
    p=buf;
    p1=(u8*)strstr((const char*)p,"$GPGSV");//????û����strstr�������***
    len=p1[7]-'0';//�õ�GPGSV������
    posx=NMEA_Comma_Pos(p1,3);//�õ��ɼ���������
    if(posx!=0XFF)gpsx->svnum=NMEA_Str2num(p1+posx,&dx);
    for(i=0;i<len;i++){
        p1=(u8*)strstr((const char*)p,"$GPGSV");//????û����strstr�������***
        for(j=0;j<4;j++){
            posx=NMEA_Comma_Pos(p1,4+j*4);
            if(posx!=0XFF)gpsx->slmsg[slx].num=NMEA_Str2num(p1+posx,&dx);//�õ����Ǳ��
            else break;
            posx=NMEA_Comma_Pos(p1,5+j*4);
            if(posx!=0XFF)gpsx->slmsg[slx].eledeg=NMEA_Str2num(p1+posx,&dx);//�õ���������
            else break;
            posx=NMEA_Comma_Pos(p1,6+j*4);
            if(posx!=0XFF)gpsx->slmsg[slx].azideg=NMEA_Str2num(p1+posx,&dx);//�õ����Ƿ�λ��
            else break;
            posx=NMEA_Comma_Pos(p1,7+j*4);
            if(posx!=0XFF)gpsx->slmsg[slx].sn=NMEA_Str2num(p1+posx,&dx);//�õ����������
            else break;
            slx++;
        }
        p=p1+1;//�л�����һ��GPGSV��Ϣ
    }
}

/******************************************************************
 *@��������NMEA����BDGSV
 *@��������������BDGSV��Ϣ������gpsx:nmea��Ϣ�ṹ��
 *@���������nmea_msg *gpsx,nmea��Ϣ�ṹ��; u8 *buf,���յ��ı������ݻ������׵�ַ
 *@����ֵ����
 ******************************************************************/
void NMEA_BDGSV_Analysis(nmea_msg *gpsx, u8 *buf){
    u8 *p,*p1,dx;
    u8 len,i,j,slx=0;
    u8 posx;
    p=buf;
    p1=(u8*)strstr((const char*)p, "$BDGSV");
    len=p1[7]-'0';//�õ�BDGSV������
    posx=NMEA_Comma_Pos(p1,3);//�õ��ɼ�������������
    if(posx!=0XFF)gpsx->beidou_svnum=NMEA_Str2num(p1+posx,&dx);
    for(i=0;i<len;i++){
        p1=(u8*)strstr((const char*)p,"$BDGSV");
        for(j=0;j<4;j++){
            posx=NMEA_Comma_Pos(p1,4+j*4);
            if(posx!=0XFF)gpsx->beidou_slmsg[slx].beidou_num=NMEA_Str2num(p1+posx,&dx); //�õ����Ǳ��
            else break;
            posx=NMEA_Comma_Pos(p1,5+j*4);
            if(posx!=0XFF)gpsx->beidou_slmsg[slx].beidou_eledeg=NMEA_Str2num(p1+posx,&dx);//�õ���������
            else break;
            posx=NMEA_Comma_Pos(p1,6+j*4);
            if(posx!=0XFF)gpsx->beidou_slmsg[slx].beidou_azideg=NMEA_Str2num(p1+posx,&dx);//�õ����Ƿ�λ��
            else break;
            posx=NMEA_Comma_Pos(p1,7+j*4);
            if(posx!=0XFF)gpsx->beidou_slmsg[slx].beidou_sn=NMEA_Str2num(p1+posx,&dx);  //�õ����������
            else break;
            slx++;
        }
        p=p1+1;//�л�����һ��BDGSV��Ϣ
    }
}

/******************************************************************
 *@��������NMEA����GNGGA
 *@��������������GNGGA��Ϣ������gpsx:nmea��Ϣ�ṹ��
 *@���������nmea_msg *gpsx,nmea��Ϣ�ṹ��; u8 *buf,���յ���GPS���ݻ������׵�ַ
 *@����ֵ����
 ******************************************************************/
void NMEA_GNGGA_Analysis(nmea_msg *gpsx,u8 *buf){
    u8 *p1,dx;
    u8 posx;
    p1=(u8*)strstr((const char *)buf,"$GNGGA");
    posx=NMEA_Comma_Pos(p1,6);                              //�õ�GPS״̬
    if(posx!=0XFF)gpsx->gpssta=NMEA_Str2num(p1+posx,&dx);
    posx=NMEA_Comma_Pos(p1,7);                              //�õ����ڶ�λ��������
    if(posx!=0XFF)gpsx->posslnum=NMEA_Str2num(p1+posx,&dx);
    posx=NMEA_Comma_Pos(p1,9);                              //�õ����θ߶�
    if(posx!=0XFF)gpsx->altitude=NMEA_Str2num(p1+posx,&dx);
}

/******************************************************************
 *@��������NMEA����GNGSA
 *@��������������GNGSA��Ϣ������gpsx:nmea��Ϣ�ṹ��
 *@���������nmea_msg *gpsx,nmea��Ϣ�ṹ��; u8 *buf,���յ���GPS���ݻ������׵�ַ
 *@����ֵ����
 ******************************************************************/
void NMEA_GNGSA_Analysis(nmea_msg *gpsx,u8 *buf){
    u8 *p1,dx;
    u8 posx;
    u8 i;
    p1=(u8*)strstr((const char *)buf,"$GNGSA");
    posx=NMEA_Comma_Pos(p1,2);                              //�õ���λ����
    if(posx!=0XFF)gpsx->fixmode=NMEA_Str2num(p1+posx,&dx);
    for(i=0;i<12;i++)                                       //�õ���λ���Ǳ��
    {
        posx=NMEA_Comma_Pos(p1,3+i);
        if(posx!=0XFF)gpsx->possl[i]=NMEA_Str2num(p1+posx,&dx);
        else break;
    }
    posx=NMEA_Comma_Pos(p1,15);                             //�õ�PDOPλ�þ�������
    if(posx!=0XFF)gpsx->pdop=NMEA_Str2num(p1+posx,&dx);
    posx=NMEA_Comma_Pos(p1,16);                             //�õ�HDOPλ�þ�������
    if(posx!=0XFF)gpsx->hdop=NMEA_Str2num(p1+posx,&dx);
    posx=NMEA_Comma_Pos(p1,17);                             //�õ�VDOPλ�þ�������
    if(posx!=0XFF)gpsx->vdop=NMEA_Str2num(p1+posx,&dx);
}

/******************************************************************
 *@��������NMEA����GNRMC
 *@��������������GNRMC��Ϣ������gpsx:nmea��Ϣ�ṹ��
 *@���������nmea_msg *gpsx,nmea��Ϣ�ṹ��; u8 *buf,���յ���GPS���ݻ������׵�ַ
 *@����ֵ����
 ******************************************************************/
void NMEA_GNRMC_Analysis(nmea_msg *gpsx,u8 *buf){
    u8 *p1,dx;
    u8 posx;
    u32 temp;
    float rs;
    p1=(u8*)strstr((const char *)buf,"$GNRMC");//"$GNRMC",������&��GNRMC�ֿ������,��ֻ�ж�GPRMC.
    posx=NMEA_Comma_Pos(p1,1);                              //�õ�UTCʱ��
    if(posx!=0XFF){
        temp=NMEA_Str2num(p1+posx,&dx)/NMEA_Pow(10,dx);     //�õ�UTCʱ��,ȥ��ms
        gpsx->utc.hour=temp/10000;
        gpsx->utc.min=(temp/100)%100;
        gpsx->utc.sec=temp%100;
    }
    posx=NMEA_Comma_Pos(p1,2);//�õ���λ���ݵ���Ч��
    if(posx!=0XFF)gpsx->gpsvalue_validity=*(p1+posx);
    posx=NMEA_Comma_Pos(p1,3);                              //�õ�γ��
    if(posx!=0XFF){
        temp=NMEA_Str2num(p1+posx,&dx);
        gpsx->latitude=temp/NMEA_Pow(10,dx+2);  //�õ���
        rs=temp%NMEA_Pow(10,dx+2);              //�õ�'
        gpsx->latitude=gpsx->latitude*NMEA_Pow(10,5)+(rs*NMEA_Pow(10,5-dx))/60;//ת��Ϊ��
    }
    posx=NMEA_Comma_Pos(p1,4);                              //��γ���Ǳ�γ
    if(posx!=0XFF)gpsx->nshemi=*(p1+posx);
    posx=NMEA_Comma_Pos(p1,5);                              //�õ�����
    if(posx!=0XFF){
        temp=NMEA_Str2num(p1+posx,&dx);
        gpsx->longitude=temp/NMEA_Pow(10,dx+2); //�õ���
        rs=temp%NMEA_Pow(10,dx+2);              //�õ�'
        gpsx->longitude=gpsx->longitude*NMEA_Pow(10,5)+(rs*NMEA_Pow(10,5-dx))/60;//ת��Ϊ��
    }
    posx=NMEA_Comma_Pos(p1,6);                              //������������
    if(posx!=0XFF)gpsx->ewhemi=*(p1+posx);
    posx=NMEA_Comma_Pos(p1,9);                              //�õ�UTC����
    if(posx!=0XFF){
        temp=NMEA_Str2num(p1+posx,&dx);                     //�õ�UTC����
        gpsx->utc.date=temp/10000;
        gpsx->utc.month=(temp/100)%100;
        gpsx->utc.year=2000+temp%100;
    }
}

/******************************************************************
 *@��������NMEA����GNVTG
 *@��������������GNVTG��Ϣ������gpsx:nmea��Ϣ�ṹ��
 *@���������nmea_msg *gpsx,nmea��Ϣ�ṹ��; u8 *buf,���յ���GPS���ݻ������׵�ַ
 *@����ֵ����
 ******************************************************************/
void NMEA_GNVTG_Analysis(nmea_msg *gpsx,u8 *buf){
    u8 *p1,dx;
    u8 posx;
    p1=(u8*)strstr((const char *)buf,"$GNVTG");
    posx=NMEA_Comma_Pos(p1,7);                              //�õ���������
    if(posx!=0XFF){
        gpsx->speed=NMEA_Str2num(p1+posx,&dx);
        if(dx<3)gpsx->speed*=NMEA_Pow(10,3-dx);             //ȷ������1000��
    }
}

/******************************************************************
 *@��������NMEA-0183
 *@������������ȡNMEA-0183������gpsx:nmea��Ϣ�ṹ��
 *@���������nmea_msg *gpsx,nmea��Ϣ�ṹ��; u8 *buf,���յ���GPS���ݻ������׵�ַ
 *@����ֵ����
 ******************************************************************/
void GPS_Analysis(nmea_msg *gpsx,u8 *buf)
{
//  NMEA_GPGSV_Analysis(gpsx,buf);  //GPGSV����
//  NMEA_BDGSV_Analysis(gpsx,buf);  //BDGSV����
//  NMEA_GNGGA_Analysis(gpsx,buf);  //GNGGA����
//  NMEA_GNGSA_Analysis(gpsx,buf);  //GPNSA����
    NMEA_GNRMC_Analysis(gpsx,buf);  //GPNMC����
//  NMEA_GNVTG_Analysis(gpsx,buf);  //GPNTG����
}



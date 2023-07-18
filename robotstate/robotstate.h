#ifndef _ROBOTSTATE_H
#define _ROBOTSTATE_H

#include "ch32v30x.h"

// �ζ�״̬ö���ͱ���
typedef enum SwimState
{
    SWIM_FORCESTOP,
    SWIM_STOP,
    SWIM_RUN,
    SWIM_INIT,
    WAIST_PITCH_STOP,
    WAIST_PITCH_RUN,
    LEFT_PECTFIN_STOP,
    LEFT_PECTFIN_RUN,
    RIGHT_PECTFIN_STOP,
    RIGHT_PECTFIN_RUN
}SwimState;

typedef struct SwimParam
{
    float penguin_swim_time;

    float pect_flap_offset[2];
    float pect_flap_amp[2];
    float pect_flap_freq[2];
    float pect_flap_angle[2];

    float pect_feather_offset[2];
    float pect_feather_amp[2];
    float pect_feather_freq[2];
    float pect_feather_angle[2];

    float pect_pitch_offset[2];
    float pect_pitch_amp[2];
    float pect_pitch_freq[2];
    float pect_pitch_angle[2];

    float tail_offset;
    float tail_amp;
    float tail_freq;
    float tail_angle;

}SwimParam;

// IMU���ݽṹ��
typedef struct GyroscopeData
{
    float roll;//�����̬��
    float pitch;//������̬��
    float yaw;//ƫ����̬��
    float accelx;//�߼��ٶ�
    float accely;//�߼��ٶ�
    float accelz;//�߼��ٶ�
    float gyrox;//���ٶ�
    float gyroy;//���ٶ�
    float gyroz;//���ٶ�
}GyroscopeData;

//������Ϣ
typedef struct power_data{
    float Voltage_data[2]; //��ѹ����
    float Current_data[2]; //��������
    float   Power_data[2];   //�й�����
    float Energy_data;  //�й��ܵ���
    float Pf_data;      //��������
    float CO2_data;     //������̼����
}pd;

//ˮ��
typedef struct pump_data
{
    float pump_angle;
    int pump_count;
}pud;

//����
typedef struct mass_data
{
    float mass_angle;
    int mass_count;
}mad;

typedef struct limit_state
{
    uint8_t pump_limit_state;
    uint8_t mass_limit_state;
}ls;

typedef struct ROBOPENGUIN
{
    float timestamp;
    SwimState swim_state;
    SwimParam swim_param;
    GyroscopeData imu_data;
    float depth;
    pd power_data;
    ls limit_state;
    int pump_distance;
    //int mass_cnt;
    int mass_distance;
    //pud pump_data;
    //mad mass_data;
    int temp_control;
}ROBOPENGUIN;

extern ROBOPENGUIN robopenguin;

#endif

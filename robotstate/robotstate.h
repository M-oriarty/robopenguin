#ifndef _ROBOTSTATE_H
#define _ROBOTSTATE_H

#include "ch32v30x.h"

// 游动状态枚举型变量
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

// IMU数据结构体
typedef struct GyroscopeData
{
    float roll;//横滚姿态角
    float pitch;//俯仰姿态角
    float yaw;//偏航姿态角
    float accelx;//线加速度
    float accely;//线加速度
    float accelz;//线加速度
    float gyrox;//角速度
    float gyroy;//角速度
    float gyroz;//角速度
}GyroscopeData;

//功率信息
typedef struct power_data{
    float Voltage_data[2]; //电压数据
    float Current_data[2]; //电流数据
    float   Power_data[2];   //有功功率
    float Energy_data;  //有功总电量
    float Pf_data;      //功率因数
    float CO2_data;     //二氧化碳排量
}pd;

//水泵
typedef struct pump_data
{
    float pump_angle;
    int pump_count;
}pud;

//滑块
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

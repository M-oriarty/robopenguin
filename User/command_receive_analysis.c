#include "command_receive_analysis.h"

void command_execute(Command command, unsigned char* message, uint8_t message_len){
    uint8_t databuf[12];
    switch(command){
        case SHAKING_HANDS:
        {
            RFLink_sendStruct(FRIEND_ID1, command, databuf, 0);//握手成功返回
        }
        break;

        case SET_SWIM_RUN:
        {
            swim_ctrl_init();
        }
        break;

        case SET_SWIM_START:
        {
            swim_ctrl_start();
        }
        break;

        case SET_SWIM_STOP:
        {
            swim_ctrl_stop();
        }
        break;

        case SET_SWIM_FORCESTOP:
        {
            swim_ctrl_forcestop();
        }
        break;

        case SET_LPECT_FLAP:
        {
            float _amp, _freq, _offset;
            rt_memcpy(&_amp,message,4);
            rt_memcpy(&_freq,message+4,4);
            rt_memcpy(&_offset,message+8,4);

            robopenguin.swim_param.pect_flap_amp[0] = _amp;
            robopenguin.swim_param.pect_flap_freq[0] = _freq;
            robopenguin.swim_param.pect_flap_offset[0] = _offset*PI;

//          SetSINAmp(&lpect_flap_sin_model, _amp);
//          SetSINFreq(&lpect_flap_sin_model, _freq);
//          SetSINOffset(&lpect_flap_sin_model, _offset);
        }
        break;

        case SET_LPECT_FEATHER:
        {
            float _amp, _freq, _offset;
            rt_memcpy(&_amp,message,4);
            rt_memcpy(&_freq,message+4,4);
            rt_memcpy(&_offset,message+8,4);

            robopenguin.swim_param.pect_feather_amp[0] = _amp;
            robopenguin.swim_param.pect_feather_freq[0] = _freq;
            robopenguin.swim_param.pect_feather_offset[0] = _offset*PI;

//          SetSINAmp(&lpect_feather_sin_model, _amp);
//          SetSINFreq(&lpect_feather_sin_model, _freq);
//          SetSINOffset(&lpect_feather_sin_model, _offset);
        }
        break;

        case SET_LPECT_PITCH:
        {
            float _amp, _freq, _offset;
            rt_memcpy(&_amp,message,4);
            rt_memcpy(&_freq,message+4,4);
            rt_memcpy(&_offset,message+8,4);

            robopenguin.swim_param.pect_pitch_amp[0] = _amp;
            robopenguin.swim_param.pect_pitch_freq[0] = _freq;
            robopenguin.swim_param.pect_pitch_offset[0] = _offset*PI;

//          SetSINAmp(&lpect_pitch_sin_model, _amp);
//          SetSINFreq(&lpect_pitch_sin_model, _freq);
//          SetSINOffset(&lpect_pitch_sin_model, _offset);
        }
        break;

        case SET_RPECT_FLAP:
        {
            float _amp, _freq, _offset;
            rt_memcpy(&_amp,message,4);
            rt_memcpy(&_freq,message+4,4);
            rt_memcpy(&_offset,message+8,4);

            robopenguin.swim_param.pect_flap_amp[1] = _amp;
            robopenguin.swim_param.pect_flap_freq[1] = _freq;
            robopenguin.swim_param.pect_flap_offset[1] = _offset*PI;

//          SetSINAmp(&rpect_flap_sin_model, _amp);
//          SetSINFreq(&rpect_flap_sin_model, _freq);
//          SetSINOffset(&rpect_flap_sin_model, _offset);
        }
        break;

        case SET_RPECT_FEATHER:
        {
            float _amp, _freq, _offset;
            rt_memcpy(&_amp,message,4);
            rt_memcpy(&_freq,message+4,4);
            rt_memcpy(&_offset,message+8,4);

            robopenguin.swim_param.pect_feather_amp[1] = _amp;
            robopenguin.swim_param.pect_feather_freq[1] = _freq;
            robopenguin.swim_param.pect_feather_offset[1] = _offset*PI;

//          SetSINAmp(&rpect_feather_sin_model, _amp);
//          SetSINFreq(&rpect_feather_sin_model, _freq);
//          SetSINOffset(&rpect_feather_sin_model, _offset);
        }
        break;

        case SET_RPECT_PITCH:
        {
            float _amp, _freq, _offset;
            rt_memcpy(&_amp,message,4);
            rt_memcpy(&_freq,message+4,4);
            rt_memcpy(&_offset,message+8,4);

            robopenguin.swim_param.pect_pitch_amp[1] = _amp;
            robopenguin.swim_param.pect_pitch_freq[1] = _freq;
            robopenguin.swim_param.pect_pitch_offset[1] = _offset*PI;

//          SetSINAmp(&rpect_pitch_sin_model, _amp);
//          SetSINFreq(&rpect_pitch_sin_model, _freq);
//          SetSINOffset(&rpect_pitch_sin_model, _offset);
        }
        break;

        case SET_TAIL:
        {
            float _amp, _freq, _offset;
            rt_memcpy(&_amp,message,4);
            rt_memcpy(&_freq,message+4,4);
            rt_memcpy(&_offset,message+8,4);

            robopenguin.swim_param.tail_amp = _amp;
            robopenguin.swim_param.tail_freq = _freq;
            robopenguin.swim_param.tail_offset = _offset*PI;

//          SetSINAmp(&tail_sin_model, _amp);
//          SetSINFreq(&tail_sin_model, _freq);
//          SetSINOffset(&tail_sin_model, _offset);
        }
        break;

        case SET_PUMP_OFF:
        {
            pump_off();
        }
        break;

        case SET_PUMP_IN:
        {
            pump_in();
        }
        break;

        case SET_PUMP_OUT:
        {
            pump_out();
        }
        break;

        case SET_LONGITUDINAL_MASS_STOP:
        {
            mass_stop();
        }
        break;

        case SET_LONGITUDINAL_MASS_FMOVE:
        {
            mass_fmove();
        }
        break;

        case SET_LONGITUDINAL_MASS_BMOVE:
        {
            mass_bmove();
        }
        break;

        case SET_DATASHOW_OVER:
        {
            set_data_type(command, 0);
            data_show_app_stop();
        }
        break;

        case READ_ROBOT_STATUS:
        {
            uint8_t rdstatus = 0x00;
          uint8_t data_buf[12];
            rdstatus = (uint8_t)(  ((robopenguin.swim_state & 0x03)<<6) + ((robopenguin.limit_state.mass_limit_state & 0x01)<<5) +
                                  ((robopenguin.limit_state.pump_limit_state & 0x01)<<4)   );
            memcpy(data_buf,&rdstatus,1);
          RFLink_sendStruct(FRIEND_ID1, command, data_buf, 1);
        }
        break;

        case READ_FLAP:
        {
            unsigned char tobesend[24];
            memcpy(&tobesend[0],&robopenguin.swim_param.pect_flap_amp[0],4);
            memcpy(&tobesend[4],&robopenguin.swim_param.pect_flap_freq[0],4);
            memcpy(&tobesend[8],&robopenguin.swim_param.pect_flap_offset[0],4);
            memcpy(&tobesend[12],&robopenguin.swim_param.pect_flap_amp[1],4);
            memcpy(&tobesend[16],&robopenguin.swim_param.pect_flap_freq[1],4);
            memcpy(&tobesend[20],&robopenguin.swim_param.pect_flap_offset[1],4);
            RFLink_sendStruct(FRIEND_ID1, command, tobesend,24);
            memset(tobesend,0,24);
        }
        break;

        case READ_FEATHER:
        {
            unsigned char tobesend[24];
            memcpy(&tobesend[0],&robopenguin.swim_param.pect_feather_amp[0],4);
            memcpy(&tobesend[4],&robopenguin.swim_param.pect_feather_freq[0],4);
            memcpy(&tobesend[8],&robopenguin.swim_param.pect_feather_offset[0],4);
            memcpy(&tobesend[12],&robopenguin.swim_param.pect_feather_amp[1],4);
            memcpy(&tobesend[16],&robopenguin.swim_param.pect_feather_freq[1],4);
            memcpy(&tobesend[20],&robopenguin.swim_param.pect_feather_offset[1],4);
            RFLink_sendStruct(FRIEND_ID1, command, tobesend,24);
            memset(tobesend,0,24);
        }
        break;

        case READ_PITCH:
        {
            unsigned char tobesend[24];
            memcpy(&tobesend[0],&robopenguin.swim_param.pect_pitch_amp[0],4);
            memcpy(&tobesend[4],&robopenguin.swim_param.pect_pitch_freq[0],4);
            memcpy(&tobesend[8],&robopenguin.swim_param.pect_pitch_offset[0],4);
            memcpy(&tobesend[12],&robopenguin.swim_param.pect_pitch_amp[1],4);
            memcpy(&tobesend[16],&robopenguin.swim_param.pect_pitch_freq[1],4);
            memcpy(&tobesend[20],&robopenguin.swim_param.pect_pitch_offset[1],4);
            RFLink_sendStruct(FRIEND_ID1, command, tobesend,24);
            memset(tobesend,0,24);
        }
        break;

        case READ_TAIL:
        {
            unsigned char tobesend[12];
            memcpy(&tobesend[0],&robopenguin.swim_param.tail_amp,4);
            memcpy(&tobesend[4],&robopenguin.swim_param.tail_freq,4);
            memcpy(&tobesend[8],&robopenguin.swim_param.tail_offset,4);
            RFLink_sendStruct(FRIEND_ID1, command, tobesend,12);
            memset(tobesend,0,12);
        }
        break;

        case READ_DEPTH:
        {
            set_data_type(command, 0);
            data_show_app_resume();
        }
        break;

        case READ_IMU_ATTITUDE:
        {
            uint8_t axis;
            memcpy(&axis,message,message_len);
            set_data_type(command, axis);
            data_show_app_resume();
        }
        break;

        case READ_IMU_ACCEL:
        {
            uint8_t axis;
            memcpy(&axis,message,message_len);
            set_data_type(command, axis);
            data_show_app_resume();
        }
        break;

        case READ_IMU_GYRO:
        {
            uint8_t axis;
            memcpy(&axis,message,message_len);
            set_data_type(command, axis);
            data_show_app_resume();
        }
        break;

        case READ_CURRENT:
        {
            set_data_type(command, 0);
            data_show_app_resume();
        }
        break;

        case READ_VOLTAGE:
        {
            set_data_type(command, 0);
            data_show_app_resume();
        }
        break;

        case READ_POWER:
        {
            set_data_type(command, 0);
            data_show_app_resume();
        }
        break;
    }
}


ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t wl_rcv_thread_stack[1024*30];
static struct rt_thread cmd_rcv_analysis_thread;

uint8_t Rx_data[50];

int command_received_length;
uint8_t command_data[12];
AnalysisResult result;

void cmd_rcv_analysis_thread_entry(void* parameter){

            if(RFLink_byteAnalysisCall(command_data)){
                    result = RFLink_commandAnalysis();
                    command_execute(result.command,result.message,result.message_len);
            }

}

void cmd_rcv_analysis_thread_create(void)
{
    rt_err_t result;

    result = rt_thread_init(&cmd_rcv_analysis_thread, "cmd_rcv_analysis",   cmd_rcv_analysis_thread_entry, RT_NULL,
                                                (rt_uint8_t*)&wl_rcv_thread_stack[0], sizeof(wl_rcv_thread_stack), 5, 60);//优先级为2，tick为10，这个比系统timer的优先级还高，需要的话可以修改

    if(result == RT_EOK)
    {
            rt_thread_startup(&cmd_rcv_analysis_thread);
    }
}

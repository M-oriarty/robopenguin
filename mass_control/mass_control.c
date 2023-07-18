#include "mass_control.h"

#define MASS_CONTROL_THREAD_PRIORITY         25
#define MASS_CONTROL_THREAD_STACK_SIZE       1024


int mass_moving_direction = 0; //0是不动，1是fmove，2是bmove

//float mass_limit_switch_angle;
//int mass_limit_switch_count;

//void set_mass_position(int position){
//  int compareValue;
//  float position_angle;
//  float angle_to_set;
//  position_angle = (float)position/100 * MASS_FULL_ANGLE;
//  angle_to_set = mass_limit_switch_angle + position_angle;
//  //printf("angle_to_set %f\n",angle_to_set);
//  while(1){
//      get_mass_final_angle(clockwise);
//      if(mass_final_angle>=angle_to_set){
//              compareValue = 1500;
//          __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_2,compareValue);
//          printf("in position\n");
//          break;
//      }else{
//              compareValue = 1700;
//          __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_2,compareValue);
//      }
//  }
//}


//method1
//void mass_init(void){
//  int compareValue;
//  get_mass_angle();
//  while(1){
//      robopenguin.limit_state.mass_limit_state = limit_detect(MASS_LIMIT);
//      if(robopenguin.limit_state.mass_limit_state == 0){
//          compareValue = 1300;
//          __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_2,compareValue);
//          get_mass_final_angle(clockwise);
//      }else if(robopenguin.limit_state.mass_limit_state == 1){
//          compareValue = 1500;
//          __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_2,compareValue);
//          get_mass_final_angle(clockwise);
//          mass_limit_switch_angle = mass_final_angle;
//          mass_limit_switch_count = mass_count;
//          break;
//      }
//  }
//}

//static void mass_control_thread_entry(void* parameter){
//  int compareValue;
//  int mass_delta_angle;
//  int mass_angle2go = MASS_FULL_ANGLE + mass_limit_switch_angle;
//  mass_init();
//  while(1){
//      mass_test();
//
//      get_mass_final_angle(clockwise);
//      if(mass_final_angle >= mass_angle2go && mass_moving_direction == 1){
//          compareValue = 1500;
//          __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_2,compareValue);
//      }else if(mass_final_angle >= mass_angle2go && mass_moving_direction == 0){
//          compareValue = 1500;
//          __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_2,compareValue);
//      }else if(robopenguin.limit_state.mass_limit_state == 1 && mass_moving_direction == 2){
//          compareValue = 1500;
//          __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_2,compareValue);
//      }else if(robopenguin.limit_state.mass_limit_state == 1 && mass_moving_direction == 0){
//          compareValue = 1500;
//          __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_2,compareValue);
//      }

//      rt_thread_delay(5);
//  }
//}

//method2
//void mass_init(void){
//  int compareValue;
//  while(1){
//      robopenguin.limit_state.mass_limit_state = limit_detect(MASS_LIMIT);
//      if(robopenguin.limit_state.mass_limit_state == 0){
//          compareValue = 1300;
//          __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_2,compareValue);
//      }else if(robopenguin.limit_state.mass_limit_state == 1){
//          compareValue = 1500;
//          __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_2,compareValue);
//          mass_limit_switch_angle = get_MASS_data();
//          while(mass_limit_switch_angle == 0){
//              mass_limit_switch_angle = get_MASS_data();
//          }
//          mass_last_angle = mass_limit_switch_angle;
//          break;
//      }
//  }
//}

//float mass_now_angle;
//float mass_last_angle;
//float mass_delta_angle;

//float mass_angle2go = MASS_FULL_ANGLE;

//static void mass_control_thread_entry(void* parameter){
//      int compareValue;
//      mass_init();
//      while(1){
//          mass_test();
//
//          mass_now_angle = get_MASS_data();
//          if(mass_now_angle == 0){
//              mass_now_angle = mass_last_angle;
//          }
//
//          if(mass_now_angle <= mass_last_angle && mass_moving_direction == 1){
//              mass_delta_angle = mass_last_angle - mass_now_angle;
//              mass_angle2go -= mass_delta_angle;
//          }else if(mass_now_angle >= mass_last_angle && mass_moving_direction == 1){
//              if(mass_now_angle - mass_last_angle <= 100){
//                  mass_delta_angle = mass_last_angle - mass_now_angle;
//              }else{
//                  mass_delta_angle = 360 - mass_now_angle + mass_last_angle;
//              }
//              mass_angle2go -= mass_delta_angle;
//          }else if(mass_now_angle <= mass_last_angle && mass_moving_direction == 2){
//              if(mass_last_angle - mass_now_angle <= 100){
//                  mass_delta_angle = mass_now_angle - mass_last_angle;
//                  mass_angle2go += mass_delta_angle;
//              }else{
//                  mass_delta_angle = 360 - mass_last_angle + mass_now_angle;
//                  mass_angle2go += mass_delta_angle;
//              }
//          }else if(mass_now_angle >= mass_last_angle && mass_moving_direction == 2){
//              mass_delta_angle = mass_now_angle - mass_last_angle;
//              mass_angle2go += mass_delta_angle;
//          }
//          mass_last_angle = mass_now_angle;
//
//          robopenguin.limit_state.mass_limit_state = limit_detect(MASS_LIMIT);
//          if(mass_angle2go <= 0 && mass_moving_direction == 1){
//              compareValue = 1500;
//              __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_2,compareValue);
//          }else if(mass_angle2go <= 0 && mass_moving_direction == 0){
//              compareValue = 1500;
//              __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_2,compareValue);
//          }else if(robopenguin.limit_state.mass_limit_state == 1 && mass_moving_direction == 2){
//              compareValue = 1500;
//              __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_2,compareValue);
//              mass_angle2go = MASS_FULL_ANGLE;
//          }else if(robopenguin.limit_state.mass_limit_state == 1 && mass_moving_direction == 0){
//              compareValue = 1500;
//              __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_2,compareValue);
//              mass_angle2go = MASS_FULL_ANGLE;
//          }
//
//          if(mass_moving_direction == 0){
//              compareValue = 1500;
//              __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_2,compareValue);
//          }
//
//          rt_thread_mdelay(5);
//      }
//}

//method3 GP2Y0E03
unsigned short int length_aveval_mass;
int mass_bm_flag;
int mass_distance;


void mass_init(void){
    int compareValue;
//  while(1){
//      SensorReadnByte_mass(0x80,(unsigned char *)&length_aveval_mass, 0x5E,2);
//      mass_distance = (R_mass[0] * 16 + R_mass[1])/16/4;
//      robopenguin.mass_distance = mass_distance;
//      if(mass_distance < mass_mid_position){
//          compareValue = 1300;
//          __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_2,compareValue);
//      }else if(mass_distance > mass_mid_position){
//          compareValue = 1700;
//          __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_2,compareValue);
//      }else if(mass_distance == mass_mid_position){
//          compareValue = 1500;
//          __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_2,compareValue);
//          break;
//      }
//  }
    SensorReadnByte_mass(0x80,(unsigned char *)&length_aveval_mass, 0x5E,2);
    mass_distance = (R_mass[0] * 16 + R_mass[1])/16/4;
    robopenguin.mass_distance = mass_distance;
    if(robopenguin.mass_distance < mass_mid_position){
        compareValue = 1300;
        TIM_SetCompare2(TIM9,compareValue);
        SensorReadnByte_mass(0x80,(unsigned char *)&length_aveval_mass, 0x5E,2);
        mass_distance = (R_mass[0] * 16 + R_mass[1])/16/4;
        robopenguin.mass_distance = mass_distance;
        while(robopenguin.mass_distance != mass_mid_position){
            SensorReadnByte_mass(0x80,(unsigned char *)&length_aveval_mass, 0x5E,2);
            mass_distance = (R_mass[0] * 16 + R_mass[1])/16/4;
            robopenguin.mass_distance = mass_distance;
            compareValue = 1300;
            TIM_SetCompare2(TIM9,compareValue);
        }
        compareValue = 1500;
        TIM_SetCompare2(TIM9,compareValue);
    }else if(robopenguin.mass_distance > mass_mid_position){
        compareValue = 1700;
        TIM_SetCompare2(TIM9,compareValue);
        SensorReadnByte_mass(0x80,(unsigned char *)&length_aveval_mass, 0x5E,2);
        mass_distance = (R_mass[0] * 16 + R_mass[1])/16/4;
        robopenguin.mass_distance = mass_distance;
        while(robopenguin.mass_distance != mass_mid_position){
            SensorReadnByte_mass(0x80,(unsigned char *)&length_aveval_mass, 0x5E,2);
            mass_distance = (R_mass[0] * 16 + R_mass[1])/16/4;
            robopenguin.mass_distance = mass_distance;
            compareValue = 1700;
            TIM_SetCompare2(TIM9,compareValue);
        }
        compareValue = 1500;
        TIM_SetCompare2(TIM9,compareValue);
    }else if(robopenguin.mass_distance == mass_mid_position){
        compareValue = 1500;
        TIM_SetCompare2(TIM9,compareValue);
    }
}

static void mass_control_thread_entry(void* parameter){
            int compareValue;
        mass_init();
        while(1){
            mass_test();

            SensorReadnByte_mass(0x80,(unsigned char *)&length_aveval_mass, 0x5E,2);
            mass_distance = (R_mass[0] * 16 + R_mass[1])/16/4;
            robopenguin.mass_distance = mass_distance;

            robopenguin.limit_state.mass_limit_state = limit_detect(MASS_LIMIT);
            if(robopenguin.limit_state.mass_limit_state == 1 || mass_distance >= mass_bm_position){
                mass_bm_flag = 1;
            }else{
                mass_bm_flag = 0;
            }


            if(mass_moving_direction == 1 && mass_distance <= mass_fm_position){
                compareValue = 1500;
                TIM_SetCompare2(TIM9,compareValue);
            }else if(mass_moving_direction == 1 && mass_distance > mass_fm_position){
                compareValue = 1700;
                TIM_SetCompare2(TIM9,compareValue);
            }else if(mass_moving_direction == 2 && mass_bm_flag == 1){
                compareValue = 1500;
                TIM_SetCompare2(TIM9,compareValue);
            }else if(mass_moving_direction == 2 && mass_bm_flag == 0){
                compareValue = 1300;
                TIM_SetCompare2(TIM9,compareValue);
            }else if(mass_moving_direction == 0){
                compareValue = 1500;
                TIM_SetCompare2(TIM9,compareValue);
            }

            rt_thread_mdelay(5);
        }
}

float time = 0;
float delta_time = 0;

//static rt_tick_t last_os_tick;

//void mass_init(void){
//  static rt_tick_t now_os_tick;//当前获取的系统tick
//
//  _mass_init();
//  now_os_tick = rt_tick_get();
//  time = (float)(now_os_tick-last_os_tick)/(float)(RT_TICK_PER_SECOND);//unit:s
//  while(time < 3.5){
//      robopenguin.mass_cnt = 0;
//      _mass_init();
//      now_os_tick = rt_tick_get();
//      time = (float)(now_os_tick-last_os_tick)/(float)(RT_TICK_PER_SECOND);//unit:s
//  }
//}

//method Optical encoders
//void _mass_init(void){
////    static rt_tick_t now_os_tick;//当前获取的系统tick
//
//  int compareValue;
//  while(1){
//      robopenguin.limit_state.mass_limit_state = limit_detect(MASS_LIMIT);
//      if(robopenguin.limit_state.mass_limit_state == 0){
//          compareValue = 1300;
//          __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_2,compareValue);
//      }else if(robopenguin.limit_state.mass_limit_state == 1){
//          compareValue = 1500;
//          __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_2,compareValue);
//          break;
//      }
//  }
//
//  __HAL_TIM_SET_CAPTUREPOLARITY(&htim2, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_RISING);
//  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_3); //启动输入捕获       或者: __HAL_TIM_ENABLE(&htim5);
//
//  last_os_tick = rt_tick_get();
//
//  while(robopenguin.mass_cnt < mass_mid_position){
//          mass_moving_direction = 1;
//          compareValue = 1700;
//          __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_2,compareValue);
//      }
//
//      mass_moving_direction = 0;
//      compareValue = 1500;
//      __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_2,compareValue);
//
////        now_os_tick = rt_tick_get();
////        time = (float)(now_os_tick-last_os_tick)/(float)(RT_TICK_PER_SECOND);//unit:s
////        delta_time = 3.5 - time;
////
////        last_os_tick = rt_tick_get();
////        while(delta_time > 0){
////            mass_moving_direction = 1;
////            compareValue = 1700;
////            __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_2,compareValue);
////            now_os_tick = rt_tick_get();
////            float temp = (float)(now_os_tick-last_os_tick)/(float)(RT_TICK_PER_SECOND);//unit:s
////            delta_time = delta_time - temp;
////        }
////        mass_moving_direction = 0;
////        compareValue = 1500;
////        __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_2,compareValue);
//      //robopenguin.mass_cnt = 80;
//
////    while(time < 3){
////        last_os_tick = rt_tick_get();
////
////        while(robopenguin.mass_cnt < mass_mid_position){
////            mass_moving_direction = 1;
////            compareValue = 1700;
////            __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_2,compareValue);
////        }
////
////        mass_moving_direction = 0;
////        compareValue = 1500;
////        __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_2,compareValue);
////
////        now_os_tick = rt_tick_get();
////        time = (float)(now_os_tick-last_os_tick)/(float)(RT_TICK_PER_SECOND);//unit:s
////    }
//}

//static void mass_control_thread_entry(void* parameter){
//  int compareValue;
//  mass_init();
//  while(1){
//      mass_test();
//      robopenguin.limit_state.mass_limit_state = limit_detect(MASS_LIMIT);
//      if(robopenguin.limit_state.mass_limit_state == 1 || robopenguin.mass_cnt == 0){
//          mass_bm_flag = 1;
//      }else{
//          mass_bm_flag = 0;
//      }
//
//      if(mass_moving_direction == 1 && robopenguin.mass_cnt < mass_max_position){
//          mass_moving_direction = 1;
//          compareValue = 1700;
//          __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_2,compareValue);
//      }else if(mass_moving_direction == 1 && robopenguin.mass_cnt > mass_max_position){
//          mass_moving_direction = 0;
//          compareValue = 1500;
//          __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_2,compareValue);
//      }else if(mass_moving_direction == 2 && mass_bm_flag == 0){
//          mass_moving_direction = 2;
//          compareValue = 1300;
//          __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_2,compareValue);
//      }else if(mass_moving_direction == 2 && mass_bm_flag == 1){
//          mass_moving_direction = 0;
//          compareValue = 1500;
//          if(robopenguin.limit_state.mass_limit_state == 1){
//              robopenguin.mass_cnt = 0;
//          }
//          __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_2,compareValue);
//      }else if(mass_moving_direction == 0){
//          mass_moving_direction = 0;
//          compareValue = 1500;
//          __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_2,compareValue);
//      }
//
//      rt_thread_mdelay(5);
//  }
//
//}

ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t mass_control_thread_stack[MASS_CONTROL_THREAD_STACK_SIZE];
static struct rt_thread mass_control_thread;

void mass_control_thread_create(void)
{

        rt_thread_init(&mass_control_thread,
                                    "_MASS_CONTROL_THREAD",
                                    mass_control_thread_entry,
                                    RT_NULL,
                                    mass_control_thread_stack,
                                    MASS_CONTROL_THREAD_STACK_SIZE,
                                    MASS_CONTROL_THREAD_PRIORITY,30);

    rt_thread_startup(&mass_control_thread);


}

void mass_stop(void){
    int compareValue;
    compareValue = 1500;
    TIM_SetCompare2(TIM9,compareValue);
    mass_moving_direction = 0;
}

void mass_fmove(void){
    int compareValue;
    compareValue = 1700;
    TIM_SetCompare2(TIM9,compareValue);
    mass_moving_direction = 1;
}

void mass_bmove(void){
    int compareValue;
    if(robopenguin.limit_state.mass_limit_state == 1){
        compareValue = 1500;
        TIM_SetCompare2(TIM9,compareValue);
    }
    compareValue = 1300;
    TIM_SetCompare2(TIM9,compareValue);
    mass_moving_direction = 2;
}

void mass_test(void){
    if(result.command == SET_LONGITUDINAL_MASS_FMOVE){
        mass_fmove();
    }else if(result.command == SET_LONGITUDINAL_MASS_BMOVE){
        mass_bmove();
    }else if(result.command == SET_LONGITUDINAL_MASS_STOP){
        mass_stop();
    }
}

#include "pump_control.h"

#define PUMP_MOVE_THREAD_PRIORITY         25
#define PUMP_MOVE_THREAD_STACK_SIZE       1024


/*　使用静态线程时，线程的栈需要设置字节对齐　*/
ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t pump_move_thread_stack[PUMP_MOVE_THREAD_STACK_SIZE];
static struct rt_thread pump_move_thread;


float pump_limit_switch_angle;
int pump_limit_switch_count;

//int pump_moving_direction = 0; //0是停止，1是in,2是out

//method1
//void pump_init(void){
//  int compareValue;
//  get_pump_angle();
//  while(1){
//      robopenguin.limit_state.pump_limit_state = limit_detect(PUMP_LIMIT);
//      if(robopenguin.limit_state.pump_limit_state == 0){
//          compareValue = 1700;
//          __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_1,compareValue);
//          get_pump_final_angle(clockwise);
//      }else if(robopenguin.limit_state.pump_limit_state == 1){
//          compareValue = 1500;
//          __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_1,compareValue);
//          get_pump_final_angle(clockwise);
//          pump_limit_switch_angle = pump_final_angle;
//          pump_limit_switch_count = pump_count;
//          break;
//      }
//
//  }
//}

//static void pump_move_entry(void* parameter)
//{
//      int compareValue;
//      int pump_angle2go = pump_limit_switch_angle - PUMP_FULL_ANGLE;
//      pump_init();
//      while(1){
//          get_pump_final_angle(clockwise);
//          if(pump_final_angle <= pump_angle2go && pump_moving_direction == 2){ //前端到达限位并向前
//              compareValue = 1500;
//              __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_1,compareValue);
//          }else if(pump_final_angle <= pump_angle2go && pump_moving_direction == 0){ //前端到达限位并停止
//              compareValue = 1500;
//              __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_1,compareValue);
//          }else if(robopenguin.limit_state.pump_limit_state == 1 && pump_moving_direction == 0){ //后端到达限位并停止
//              compareValue = 1500;
//              __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_1,compareValue);
//          }else if(robopenguin.limit_state.pump_limit_state == 1 && pump_moving_direction == 1){ //后端到达限位并向后
//              compareValue = 1500;
//              __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_1,compareValue);
//          }

//          rt_thread_delay(5);
//  }
//}

//float pump_now_angle;
//float pump_last_angle;
//float pump_delta_angle;


////method2
//void pump_init(void){
//  int compareValue;
//  while(1){
//      robopenguin.limit_state.pump_limit_state = limit_detect(PUMP_LIMIT);
//      if(robopenguin.limit_state.pump_limit_state == 0){
//          compareValue = 1700;
//          __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_1,compareValue);
//      }else if(robopenguin.limit_state.pump_limit_state == 1){
//          compareValue = 1500;
//          __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_1,compareValue);
//          pump_limit_switch_angle = get_PUMP_data();
//          while(pump_limit_switch_angle == 0){
//              pump_limit_switch_angle = get_PUMP_data();
//          }
//          pump_last_angle = pump_limit_switch_angle;
//          break;
//      }
//  }
//}

//float pump_angle2go = PUMP_FULL_ANGLE;
//float pump_mid2go = PUMP_MID_ANGLE;

//void pump_set_mid(void){
//  int compareValue;
//  compareValue = 1300;
//  __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_1,compareValue);
//  pump_moving_direction = 2;
//  while(1){
//      pump_now_angle = get_PUMP_data();
//      if(pump_now_angle == 0){
//          pump_now_angle = pump_last_angle;
//      }
//
//      if((pump_now_angle+pump_last_angle)/2 - pump_last_angle >= 30 && pump_moving_direction == 2){
//          pump_now_angle = pump_last_angle;
//      }
//
//      if(pump_now_angle >= pump_last_angle && pump_moving_direction == 2){
//          pump_delta_angle = pump_now_angle - pump_last_angle;
//          pump_mid2go -=pump_delta_angle;
//          pump_angle2go -= pump_delta_angle;
//      }else if(pump_now_angle < pump_last_angle && pump_moving_direction == 2){
//          if(pump_last_angle - pump_now_angle <= 100){
//              pump_delta_angle = pump_now_angle - pump_last_angle;
//              pump_angle2go -= pump_delta_angle;
//          }else{
//              pump_delta_angle = 360 - pump_last_angle + pump_now_angle;
//              pump_mid2go -= pump_delta_angle;
//              pump_angle2go -= pump_delta_angle;
//          }
//      }
//
//      pump_last_angle = pump_now_angle;
//
//      if(pump_angle2go <= 0 && pump_moving_direction == 2){
//          compareValue = 1500;
//          __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_1,compareValue);
//          break;
//      }
//
//  }
//
//}


//static void pump_move_entry(void* parameter)
//{
//      int compareValue;
//      pump_init();
//      //pump_set_mid();
//      while(1){
//          if(pump_angle2go >= PUMP_FULL_ANGLE){
//              pump_angle2go = PUMP_FULL_ANGLE;
//          }
//          pump_test();
//
//          pump_now_angle = get_PUMP_data();
//          if(pump_now_angle == 0){
//              pump_now_angle = pump_last_angle;
//          }
//
//          if((pump_now_angle+pump_last_angle)/2 - pump_last_angle >= 30 && pump_moving_direction == 2){
//              pump_now_angle = pump_last_angle;
//          }else if(pump_last_angle-(pump_now_angle+pump_last_angle)/2>= 30 && pump_moving_direction == 1){
//              pump_now_angle = pump_last_angle;
//          }
//
//          if(pump_now_angle >= pump_last_angle && pump_moving_direction == 2){
//              pump_delta_angle = pump_now_angle - pump_last_angle;
//              pump_angle2go -= pump_delta_angle;
//          }else if(pump_now_angle < pump_last_angle && pump_moving_direction == 2){
//              if(pump_last_angle - pump_now_angle <= 100){
//                  pump_delta_angle = pump_now_angle - pump_last_angle;
//                  pump_angle2go -= pump_delta_angle;
//              }else{
//                  pump_delta_angle = 360 - pump_last_angle + pump_now_angle;
//                  pump_angle2go -= pump_delta_angle;
//              }
//          }else if(pump_now_angle <= pump_last_angle && pump_moving_direction == 1){
//              pump_delta_angle = pump_last_angle - pump_now_angle;
//              pump_angle2go += pump_delta_angle;
//          }else if(pump_now_angle > pump_last_angle && pump_moving_direction == 1){
//              if(pump_now_angle - pump_last_angle <= 100){
//                  pump_delta_angle = pump_last_angle - pump_now_angle;
//                  pump_angle2go += pump_delta_angle;
//              }else{
//                  pump_delta_angle = 360 - pump_now_angle + pump_last_angle;
//                  pump_angle2go += pump_delta_angle;
//              }
//          }
//          pump_last_angle = pump_now_angle;
//
//          robopenguin.limit_state.pump_limit_state = limit_detect(PUMP_LIMIT);
//          if(pump_angle2go <= 0 && pump_moving_direction == 2){
//              compareValue = 1500;
//              __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_1,compareValue);
//              pump_off();
//          }else if(pump_angle2go <= 0 && pump_moving_direction == 0){
//              compareValue = 1500;
//              __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_1,compareValue);
//          }else if(robopenguin.limit_state.pump_limit_state == 1 && pump_moving_direction == 1){
//              compareValue = 1500;
//              __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_1,compareValue);
//              pump_angle2go = PUMP_FULL_ANGLE;
//          }else if(robopenguin.limit_state.pump_limit_state == 1 && pump_moving_direction == 0){
//              compareValue = 1500;
//              __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_1,compareValue);
//              pump_angle2go = PUMP_FULL_ANGLE;
//          }

//          rt_thread_mdelay(5);
//  }
//}

int pump_moving_direction = 0; //0是不动，1是fmove，2是bmove
unsigned short int length_aveval_pump;
int pump_bm_flag;
int pump_distance;
void pump_init(void){
    int compareValue;
//  while(1){
//      SensorReadnByte_pump(0x80,(unsigned char *)&length_aveval_pump, 0x5E,2);
//      pump_distance = (R_PUMP[0] * 16 + R_PUMP[1])/16/4;
//      robopenguin.pump_distance = pump_distance;
////        printf("pump distance %d\n",pump_distance);
//      if(pump_distance >= 30){
//          compareValue = 1300;
//          __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_1,compareValue);
//      }else if(pump_distance < pump_mid_position){
//          compareValue = 1300;
//          __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_1,compareValue);
//      }else if(pump_distance > pump_mid_position){
//          compareValue = 1700;
//          __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_1,compareValue);
//      }else if(pump_distance == pump_mid_position){
//          compareValue = 1500;
//          __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_1,compareValue);
//          break;
//      }
//  }
    SensorReadnByte_pump(0x80,(unsigned char *)&length_aveval_pump, 0x5E,2);
    pump_distance = (R_PUMP[0] * 16 + R_PUMP[1])/16/4;
    robopenguin.pump_distance = pump_distance;
    if(robopenguin.pump_distance >= 30 || robopenguin.pump_distance < pump_mid_position){
        compareValue = 1300;
        TIM_SetCompare1(TIM9,compareValue);
        SensorReadnByte_pump(0x80,(unsigned char *)&length_aveval_pump, 0x5E,2);
        pump_distance = (R_PUMP[0] * 16 + R_PUMP[1])/16/4;
        robopenguin.pump_distance = pump_distance;
        while(robopenguin.pump_distance != pump_mid_position){
            SensorReadnByte_pump(0x80,(unsigned char *)&length_aveval_pump, 0x5E,2);
            pump_distance = (R_PUMP[0] * 16 + R_PUMP[1])/16/4;
            robopenguin.pump_distance = pump_distance;
            compareValue = 1300;
            TIM_SetCompare1(TIM9,compareValue);
        }
        compareValue = 1500;
        TIM_SetCompare1(TIM9,compareValue);
    }else if(robopenguin.pump_distance > pump_mid_position){
        compareValue = 1700;
        TIM_SetCompare1(TIM9,compareValue);
        SensorReadnByte_pump(0x80,(unsigned char *)&length_aveval_pump, 0x5E,2);
        pump_distance = (R_PUMP[0] * 16 + R_PUMP[1])/16/4;
        robopenguin.pump_distance = pump_distance;
        while(robopenguin.pump_distance != pump_mid_position){
            SensorReadnByte_pump(0x80,(unsigned char *)&length_aveval_pump, 0x5E,2);
            pump_distance = (R_PUMP[0] * 16 + R_PUMP[1])/16/4;
            robopenguin.pump_distance = pump_distance;
            compareValue = 1700;
            TIM_SetCompare1(TIM9,compareValue);
        }
        compareValue = 1500;
        TIM_SetCompare1(TIM9,compareValue);
    }else if(robopenguin.pump_distance == pump_mid_position){
        compareValue = 1500;
        TIM_SetCompare1(TIM9,compareValue);
    }
}

static void pump_move_entry(void* parameter){
    int compareValue;
    pump_init();
    while(1){
        pump_test();
        robopenguin.limit_state.pump_limit_state = limit_detect(PUMP_LIMIT);
        if(robopenguin.limit_state.pump_limit_state == 1 || pump_distance <= pump_bm_position || pump_distance >= 30){
                pump_bm_flag = 1;
            }else{
                pump_bm_flag = 0;
            }

            if(pump_moving_direction == 1 && pump_distance >= pump_fm_position && pump_distance < 30){
                compareValue = 1500;
                TIM_SetCompare1(TIM9,compareValue);
            }else if( (pump_moving_direction == 1 && pump_distance < pump_fm_position) || (pump_moving_direction == 1 && pump_distance >= 30) ){
                compareValue = 1300;
                TIM_SetCompare1(TIM9,compareValue);
            }else if(pump_moving_direction == 2 && pump_bm_flag == 1){
                compareValue = 1500;
                TIM_SetCompare1(TIM9,compareValue);
            }else if(pump_moving_direction == 2 && pump_bm_flag == 0){
                compareValue = 1700;
                TIM_SetCompare1(TIM9,compareValue);
            }else if(pump_moving_direction == 0){
                compareValue = 1500;
                TIM_SetCompare1(TIM9,compareValue);
            }

            rt_thread_mdelay(5);

    }
}


void pump_move_thread_create(void)
{

    rt_thread_init(&pump_move_thread,
                                    "_PUMP_MOVE_UPDATE",
                                    pump_move_entry,
                                    RT_NULL,
                                    pump_move_thread_stack,
                                    PUMP_MOVE_THREAD_STACK_SIZE,
                                    PUMP_MOVE_THREAD_PRIORITY,30);

    rt_thread_startup(&pump_move_thread);

}


//void set_pump_position(int position){
//  int compareValue;
//  float position_angle;
//  float angle_to_set;
//  position_angle = (float)position/100 * PUMP_FULL_ANGLE;
//  angle_to_set = pump_limit_switch_angle - position_angle;
//  //printf("angle to set %f\n",angle_to_set);
//  while(1){
//      get_pump_final_angle(clockwise);
//      //printf("now angle %f\n",pump_last_angle);
//      if(pump_final_angle<=angle_to_set){
//              compareValue = 1500;
//          __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_1,compareValue);
//          //printf("in position\n");
//          break;
//      }else{
//              compareValue = 1300;
//          __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_1,compareValue);
//      }
//  }
//}

void pump_off(void){
    int compareValue;
    compareValue = 1500;
    TIM_SetCompare1(TIM9,compareValue);
    pump_moving_direction = 0;
}

void pump_in(void){
    int compareValue;
    if(robopenguin.limit_state.pump_limit_state == 1){
        compareValue = 1500;
        TIM_SetCompare1(TIM9,compareValue);
    }else{
        compareValue = 1700;
        TIM_SetCompare1(TIM9,compareValue);
        pump_moving_direction = 2;
    }
}

void pump_out(void){
    int compareValue;
    compareValue = 1300;
    TIM_SetCompare1(TIM9,compareValue);
    pump_moving_direction = 1;
}

void pump_test(void){
    if(result.command == SET_PUMP_IN){
        pump_in();
    }else if(result.command == SET_PUMP_OUT){
        pump_out();
    }else if(result.command == SET_PUMP_OFF){
        pump_off();
    }
}

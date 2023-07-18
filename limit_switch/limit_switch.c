#include "limit_switch.h"

uint8_t limit_detect(uint8_t limit_id)
{
    uint8_t pump_limit_state = 0;
    uint8_t mass_limit_state = 0;

    if(limit_id == PUMP_LIMIT){
        if(GPIO_ReadInputDataBit(PUMP_LIMIT_GPIO,PUMP_LIMIT_PIN) == 1){
                pump_limit_state = 1;
        }else{
            return pump_limit_state;
        }
    }else if(limit_id == MASS_LIMIT){
        if(GPIO_ReadInputDataBit(MASS_LIMIT_GPIO,MASS_LIMIT_PIN) == 1){
                mass_limit_state = 1;
        }else{
            return mass_limit_state;
        }
    }
}

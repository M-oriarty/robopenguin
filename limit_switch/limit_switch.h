#ifndef _LIMIT_SW_CTRL_H
#define _LIMIT_SW_CTRL_H

#include <rtthread.h>
#include <stdbool.h>
#include "mass_control.h"

#define PUMP_LIMIT       0
#define PUMP_LIMIT_GPIO  GPIOE
#define PUMP_LIMIT_PIN   GPIO_Pin_12

#define MASS_LIMIT       1
#define MASS_LIMIT_GPIO  GPIOE
#define MASS_LIMIT_PIN   GPIO_Pin_11

uint8_t limit_detect(uint8_t limit_id);

#endif

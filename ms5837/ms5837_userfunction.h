#ifndef __MS5837_USERFUNCTION_H
#define __MS5837_USERFUNCTION_H

#include <stdbool.h>
#include "delay.h"
#include "ms5837.h"
#include "usart.h"
#include <stdio.h>
#include "rtthread.h"

void ms5837_init(void);
void ms5837_reset(void);
float get_ms5837_data(void);



#endif

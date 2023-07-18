#ifndef _SENSORS_UPDATE_H
#define _SENSORS_UPDATE_H

#include "rtthread.h"
#include "ms5837_userfunction.h"
#include "JY_901_userfunction.h"
#include "JY_901.h"
#include "robotstate.h"
#include "IM1253B_userfunction.h"
#include "limit_switch.h"
//#include "communicate.h"
#include "pump_control.h"
#include "mass_control.h"
#include "command_receive_analysis.h"
#include "wk2124.h"

static void sensors_entry(void* parameter);
void sensors_thread_create(void);

#endif

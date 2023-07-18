#ifndef _DATA_SHOW_APP_H
#define _DATA_SHOW_APP_H

#include "stdio.h"
#include "command_receive_analysis.h"
#include "robotstate.h"
#include "rflink.h"

void set_data_type(Command command, uint8_t data);
void data_show_thread_tmrcallback(void* parameter);
static void data_show_thread_entry(void* parameter);
void data_show_thread_create(void);
void data_show_app_stop(void);
void data_show_app_resume(void);

#endif

#ifndef _COMMAND_ANALYSIS_H
#define _COMMAND_ANALYSIS_H

#include "rtthread.h"
#include "stdio.h"
#include "robotstate.h"
#include "rflink.h"
#include "sin.h"
#include "data_show_app.h"
#include "swim_control.h"

extern uint8_t Rx_data[50];

extern int command_received_length;
extern uint8_t command_data[12];

extern AnalysisResult result;

void command_execute(Command command, unsigned char* message, uint8_t message_len);

void cmd_rcv_analysis_thread_create(void);

#endif

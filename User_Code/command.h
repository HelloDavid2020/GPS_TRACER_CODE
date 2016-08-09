#ifndef _COMMAND_H_ 
#define _COMMAND_H_

#include "includes.h"

typedef enum 
{
	InputSend, 
	DebugGSM, 
	DebugGPS, 
	Command, 
	Modbus,
} UartMode;	

extern UartMode g_uartMode;

extern void DebugLog(char *c);
void DebugData(char *c, uint32_t length);
void InputCommand(char c);

extern void ascii2data(char * des, char * src, int size);

extern void GetTimer(void);
extern void SetTimer(char *c, int8u length);

#endif


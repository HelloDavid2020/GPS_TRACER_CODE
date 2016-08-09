#ifndef __USER_DEBUGMSG_H
#define __USER_DEBUGMSG_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "includes.h"

extern void Debug_TCPIP_Inputdata(void);
extern void Debug_GSM(void);

	 
extern void ShowVehicleMsg(void);
extern void ShowStatus(void);
extern void ShowModbus5(void);	 
	 
extern uint32_t debugTimeCount;
	 
#ifdef __cplusplus	 
} 
#endif
 
#endif


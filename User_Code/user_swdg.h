#ifndef __USER_SWDG_H
#define __USER_SWDG_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "includes.h"
	 
typedef enum
{
	TASK2_SWDG_ID = 0,
	TASK3_SWDG_ID = 1,
	MAX_SWDG_ID
}SWDG_ID;
	 
// Èí¼þ ¿´ÃÅ¹·×´Ì¬ 
typedef enum
{
	SWDT_STAT_IDLE =0, 
	SWDT_STAT_SUSPEN,
	SWDT_STAT_RUN
}SWDT_STAT;
 

typedef struct 
{
	uint16_t   TimeOut; // 
	uint16_t   TimerCount; 
	SWDT_STAT  RunState;  
}SOFT_WATCH_DOG_TIMER;


extern SOFT_WATCH_DOG_TIMER SoftTimerCountrList[MAX_SWDG_ID];
extern BOOL Feed_WDG_Flag;
extern void SoftWdgStateInit(void);
extern BOOL SoftWdtInit(SWDG_ID SwdtId, uint16_t timeout);
extern void FeedSoftWdg(SWDG_ID SwdtId);
extern void FeedDog(void);
extern void SoftWdgHandler(void);


#endif




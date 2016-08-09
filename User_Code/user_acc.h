#ifndef __USER_ACC_H
#define __USER_ACC_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "includes.h"
	 
typedef struct
{
	uint8_t PrevVal;
	uint8_t CurrentVal;
	uint8_t ChangFlag;
} STATE_CHANGE;	 
extern STATE_CHANGE AccDetect;
extern STATE_CHANGE MotoDetect;

typedef struct
{
	uint8_t year;
	uint8_t mon;
	uint8_t day;
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
} RTC_DATE;

typedef struct
{
	uint8_t  NewFlag; // 新的一天标志
	
	uint8_t  PrevYear;
	uint8_t  PrevMon;
	uint8_t  PrevDay;
	uint16_t DayOfPrevYear; // 
	
	uint8_t  CurentYear;
	uint8_t  CurentMon;
	uint8_t  CurentDay;	
	uint16_t DayOfCurentYear; // 
} CREAT_FLAG_OF_A_NEW_DAY;

extern CREAT_FLAG_OF_A_NEW_DAY CheckNewDay;

extern uint8_t FlagOfaNewDay[2];
extern RTC_DATE RtcDate;
	 
extern void cleanAcc(void);
extern void ReportAcc(void);
extern void TaskTimeAcc(void);

extern void CheckAccAndMoto(void);
	 
extern void SaveLastEngineStartTime(void);
extern void SaveFirstEngineStartTime(void);	 
extern void GetFirstEngineStartTime(RTC_DATE *rtc_date);	 
 

extern void GetFlagOfaNewDay(void);
extern uint16_t GetVehicleWorkTimeOfOneDay(void);

#ifdef __cplusplus
}
#endif 

#endif







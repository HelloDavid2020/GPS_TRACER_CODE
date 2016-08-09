#ifndef __RTC_H
#define	__RTC_H

#include "includes.h"


/* 如果定义了下面这个宏的话,PC13就会输出频率为RTC Clock/64的时钟 */   
//#define RTCClockOutput_Enable  /* RTC Clock/64 is output on tamper pin(PC.13) */

extern u32 rtc_counter;
extern u8 SetTimeFlag;
extern u8 SetAlarmTimeFlag;

extern time_t current_time;
extern struct tm time_now;
extern struct tm time_set;
extern __IO uint32_t TimeDisplay;	

extern struct tm Time_ConvUnixToCalendar(time_t t);
extern time_t Time_ConvCalendarToUnix(struct tm t);
extern time_t Time_GetUnixTime(void);
extern struct tm Time_GetCalendarTime(void);
extern void Time_SetUnixTime(time_t);
extern void Time_SetCalendarTime(struct tm t);

extern void dis_rtc(void);
extern void RtcInit(void);
extern void NVIC_Configuration(void);
extern void RTC_Configuration(void);
extern uint32_t Time_Regulate(void);
//extern void Time_Adjust(void);
extern void Time_Display(uint32_t TimeVar);
extern void Time_Show(void);
extern uint8_t USART_Scanf(uint32_t value);
extern void SetAlarmTime(void);

#endif /* __XXX_H */

#ifndef _USER_TIMEDELAY_H
#define _USER_TIMEDELAY_H

#include "includes.h"
extern __IO uint32_t TimingDelay;
extern void User_DelayuS(uint32_t delay) ;
extern void DelayMS(uint32_t delayms);
extern void delay_ms(uint32_t delayms);
extern void delay(u16 cnt);
extern void delay_us(__IO uint32_t nTime);
extern void Init_SysTick(void);



#endif



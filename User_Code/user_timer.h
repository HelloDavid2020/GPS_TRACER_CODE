#ifndef __USER_TIMER_H
#define __USER_TIMER_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "includes.h"	 
	 
#define START_TIMER3() 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);TIM_Cmd(TIM3, ENABLE)
#define STOP_TIMER3()  	TIM_Cmd(TIM3, DISABLE);RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , DISABLE)

	 
extern uint32_t Timer2_count;
	
extern void User_Timer2_Configuration(void);
extern void User_Timer3_Configuration(void);
	 
#ifdef __cplusplus
}
#endif 

#endif


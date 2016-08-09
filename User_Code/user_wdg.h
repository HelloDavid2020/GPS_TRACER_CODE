#ifndef __USER_WDG_H
#define __USER_WDG_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "includes.h"
	 
#include <stm32f10x.h>
#include <system_stm32f10x.h>

	 
#include "stm32f10x_iwdg.h"
#include "stm32f10x_wwdg.h"	 
#include "stm32f10x_exti.h"	 
	 
	 
	 
extern void User_IWDG_Init(uint16_t timeout);
extern void User_WWDG_Init(void);	 
	 
extern void User_Feed_IWDG(void);
extern void User_Feed_WWDG(void);	 
extern void User_Close_WWDG(void);
	 
#ifdef __cplusplus
}
#endif	 	 
	 
#endif	 



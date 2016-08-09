#ifndef _USER_NVIC_H
#define _USER_NVIC_H

#include "stm32f10x.h"


extern void User_NVIC_Configuration(void);
extern void IntEnable(void);
extern void IntDisable(void);

#endif


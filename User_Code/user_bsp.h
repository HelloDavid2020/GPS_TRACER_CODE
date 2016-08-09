#include "includes.h"

#ifndef __USER_BSP_H_ 
#define __USER_BSP_H_


#ifndef OK
#define OK			1 
#endif

#ifndef ERROR
#define ERROR		0    
#endif

#ifndef TRUE
#define TRUE		1
#endif

#ifndef FALSE
#define FALSE		0  
#endif

#ifndef NULL
#define NULL		0ul
#endif 	

/*********************************************************************************************************
  包含需要用到的BSP驱动，用到的定义为真，否则为假
*********************************************************************************************************/
#define   OS_UCOSII_EN           1
#define   BSP_GPIO_EN            1
#define   BSP_TIMER_EN           1
#define   BSP_SPIFLASH_EN		 1
#define   BSP_QUEUE_EN			 1
#define   BSP_UART_EN			 1
/*********************************************************************************************************
  根据条件包含需要用到的BSP驱动
*********************************************************************************************************/
// #if (OS_UCOSII_EN > 0)   
// 	#include "ucos_ii.h"            
// #endif

// #if (BSP_TIMER_EN > 0)   
// 	#include "bsp_timer.h"             
// #endif

// #if (BSP_SPIFLASH_EN > 0)   
// 	#include "bsp_spiflash.h"             
// #endif

// #if (BSP_QUEUE_EN > 0)   
// 	#include "bsp_queue.h"             
// #endif

// #include "bsp_i2c_eerom.h"


#include "stm32f10x_it.h"
#include "MX25_CMD.h"

extern uint8_t g_needReportAlarm;

extern void BspInit(void);
extern void LoadHomeTcpAdd(void);

extern void DisableInt(void);
extern void EnableInt(void);

extern uint32_t ChipUniqueID[3];
extern RCC_ClocksTypeDef RCC_Clock;



#endif	 //__BSP_H_



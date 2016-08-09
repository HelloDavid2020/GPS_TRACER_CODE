#include "user_clock.h"

// 使用内部时钟初始化 8M
void SystemClockInit(void)
{ 
	RCC_DeInit();

	RCC_HSEConfig(RCC_HSE_OFF);  
	RCC_HSICmd(ENABLE); 
  RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
	
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET)        
	{        
	}
	
  /* Enable Prefetch Buffer */
  FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    
	/* Flash 2 wait state */
  FLASH_SetLatency(FLASH_Latency_2);
	
	
	/* 设置AHB时钟 HCLK */
	/* HCLK = SYSCLK */
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	

	/* 设置高速AHB时钟 PLCLK2 */
	//APB2
	RCC_PCLK2Config(RCC_HCLK_Div1);

	/* 设置低速AHB时钟 PLCLK1 */
	//APB1
	RCC_PCLK1Config(RCC_HCLK_Div2);


	 /* PLLCLK = 4MHz * 9 = 36 MHz */
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_9);     // 
	
	/* Enable PLL */ 
	RCC_PLLCmd(ENABLE); 

	/* Wait till PLL is ready */
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
	{
	}
	
	/* Select PLL as system clock source */
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);        

	/* Wait till PLL is used as system clock source */
  while(RCC_GetSYSCLKSource() != 0x08){}
}

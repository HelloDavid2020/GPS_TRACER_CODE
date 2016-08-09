#include "user_wdg.h"

// timeout 单位 ms
void User_IWDG_Init(uint16_t timeout)
{
	uint16_t time_count = 0;
	
  /* Check if the system has resumed from IWDG reset */
  if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
  {
    /* Clear reset flags */
    RCC_ClearFlag();
  }
	
  /* IWDG timeout equal to 280 ms (the timeout may varies due to LSI frequency
     dispersion) */
  /* Enable write access to IWDG_PR and IWDG_RLR registers */
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

/* IWDG counter clock: 40KHz(LSI) / 32 = 1.25 KHz, (0.8 ms) */
//  IWDG_SetPrescaler(IWDG_Prescaler_4);  // 0.100 mS
  IWDG_SetPrescaler(IWDG_Prescaler_32); // 0.800 mS	
//  IWDG_SetPrescaler(IWDG_Prescaler_64); // 1.6 mS


	time_count = timeout + (timeout >>2) -1;
	
	
  /* Set counter reload value to 624  (between 0 and 4095) */
  IWDG_SetReload(time_count); // (624+1)*0.8 =500 mS

  /* Reload IWDG counter */
  IWDG_ReloadCounter();

  /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
  IWDG_Enable();	
	
}
	
void User_WWDG_Init(void)
{
	#ifdef ENABLE_WWDG
	uint8_t window_value = 0x6F;
	/* Check if the system has resumed from WWDG reset */
  if (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) != RESET)
  { 
		/* Clear reset flags */
    RCC_ClearFlag();
  }

  /* WWDG configuration */
  /* Enable WWDG clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);

  /* WWDG clock counter = (PCLK1/4096)/8 = 244 Hz (~4 ms)  */
  WWDG_SetPrescaler(WWDG_Prescaler_8);

  /* Set Window value to 65 */
  WWDG_SetWindowValue(window_value);

	/* Enable WWDG and set counter value to 127, WWDG timeout = ~4 ms * (window_value-1) */

  /* Enable WWDG and set counter value to 127, WWDG timeout = ~4 ms * 64 = 262 ms */
  WWDG_Enable(127);

  /* Clear EWI flag */
  WWDG_ClearFlag();

  /* Enable EW interrupt */
  WWDG_EnableIT();	
	#endif
}

//
void User_Feed_IWDG(void)
{
  /* Reload IWDG counter */
  IWDG_ReloadCounter();
}


//
void User_Feed_WWDG(void)
{
	#ifdef ENABLE_WWDG
	uint8_t wr = 0;
	uint8_t tr = 0;

	wr=WWDG->CFR&0X7F;  // W[6:0]即是WWDG->CFR的低七位，是窗口看门狗的上窗口，下窗口是0x40
	tr=WWDG->CR &0X7F;
	if(tr<wr)
	{
		/* Update WWDG counter */
		WWDG_SetCounter(0x7F);

		/* Clear EWI flag */
		WWDG_ClearFlag();
	}
	#endif
}

// 函数测试无效
void User_Close_WWDG(void)
{
	#ifdef ENABLE_WWDG
	
	User_Feed_WWDG();
	WWDG_DeInit();
	
	// disable wwdg interrupt
__disable_irq(); 
	
	#endif
}


	

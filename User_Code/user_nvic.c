#include "user_nvic.h"


/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures NVIC and Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void User_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

#ifdef  VECT_TAB_RAM
  /* Set the Vector Table base location at 0x20000000 */
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */
	// 0x08042C00
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x00);
#endif

  /* 2 bits for Preemption Priority and 2 bits for Sub Priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

   NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);


	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	


//	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;	  
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 8;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);	

//	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;	  
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 9;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);	

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 9;	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	


}

void IntDisable(void)
{

	USART_ITConfig(USART1, USART_IT_TXE | USART_IT_RXNE, DISABLE);
	USART_ITConfig(USART2, USART_IT_TXE | USART_IT_RXNE, DISABLE);
	USART_ITConfig(USART3, USART_IT_TXE | USART_IT_RXNE, DISABLE);
	USART_ITConfig(UART4, USART_IT_TXE | USART_IT_RXNE, DISABLE);
	
	
//	CAN_ITConfig(CAN1, CAN_IT_FMP0, DISABLE);
//	TIM_ITConfig(TIM1,TIM_IT_Update,DISABLE);

	TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);
	TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);
	TIM_ITConfig(TIM4,TIM_IT_Update,DISABLE);
	TIM_ITConfig(TIM5,TIM_IT_Update,DISABLE);
}

void IntEnable(void)
{

	USART_ITConfig(USART1, USART_IT_TXE | USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART2, USART_IT_TXE | USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART3, USART_IT_TXE | USART_IT_RXNE, ENABLE);
	USART_ITConfig(UART4, USART_IT_TXE | USART_IT_RXNE, ENABLE);
	
	
//	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
//	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);

	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);

}


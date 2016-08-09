/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include <stdio.h>
#include <string.h>

#include <stm32f10x.h>
#include <system_stm32f10x.h>
#include "includes.h"


/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/


/*******************************************************************************
* Function Name  : WWDG_IRQHandler
* Description    : This function handles WWDG interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void WWDG_IRQHandler(void)
{
  /* Update WWDG counter */
  WWDG_SetCounter(0x7F); // 

// -----------------------------------------------------------------------------
// add user code
/* Toggle GPIO_Led pin 3 */
//	GPIO_Toggle(GPIOE,LED_RED);
// ------------------------------------------------------------------------------
/* Clear EWI flag */
  WWDG_ClearFlag();

}
/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
	printf("> HardFault!\r\n");
  while (1)
  {
  }
}


/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
//void SVC_Handler(void)
//{
//}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}




/*******************************************************************************
* Function Name  : RTCAlarm_IRQHandler
* Description    : This function handles RTC Alarm interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTCAlarm_IRQHandler(void)
{
	
	
}



// ▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇		
// 频率采集 250ms

void TIM2_IRQHandler(void)
{
	if ( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET )
	{
		if (TimingDelay != 0x00)
		{ 
			TimingDelay--;
		}
TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);	  //清除中断标志
	}
}

/*******************************************************************************
* Function Name  : EXTI0_IRQHandler
* Description    : This function handles External interrupt Line 0 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI0_IRQHandler(void)  /* Key 4 */
{
	
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
    /* Clear the EXTI Line 0 */
    EXTI_ClearITPendingBit(EXTI_Line0);
		NVIC_SystemReset(); 
		
		
  }
}


// ▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇		
// 1mS定时中断 
void TIM3_IRQHandler(void)
{
//	static uint16_t can_tx_timer = 0;
	if ( TIM_GetITStatus(TIM3 , TIM_IT_Update) != RESET ) 
	{	
		AdcFilter();
//		DetectPortInput(); 				// 检测端口输入
//		OutputControl(ExOutput);	// 控制端口输出
		TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update);    

	}		 	
}


// 定时器TIM4的中断
// 串口通信超时判断
//extern void prvvTIMERExpiredISR( void );

void TIM4_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
  {
    /* Clear TIM4 Capture Compare1 interrupt pending bit*/
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
//		AdcFilter();
  }
}


// ▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
	uint8_t cmd_rx_buf[100];

void USART1_IRQHandler(void)
{
	uint8_t queue_read_buf;

	if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET)   	//接收中断
	{
		time_auto_pwr_off = CNT_POWER_OFF;
		
		queue_read_buf=USART_ReceiveData(USART1);
		if(flag_csq_rx==0)		
		{
			csq_rx[csq_rx_len]=queue_read_buf;
			csq_rx_len++;
			if(csq_rx_len>1000)
			{
				csq_rx_len=0;
				flag_csq_rx=1;
			}
		}
		SerialPutChar(queue_read_buf); // 转发给电脑
		
		
		UART1RecvBuf[usart1_rx_len]=queue_read_buf;
		
//---------------------------------------------------------------------------
		if(UART1RecvBuf[usart1_rx_len]==0x0A)
		{ 
			flag_usart1_rx_done=1;
		}
		else
		{
			if(flag_usart1_rx_done==0)
			usart1_rx_len++;
			if(usart1_rx_len >= UART1_RECV_QUEUE_LENGTH)	usart1_rx_len = 0;
		}
//---------------------------------------------------------------------------

		if(en_info_rx==1)
		{
			info_rx[info_rx_len]=queue_read_buf;
			info_rx_len++;
			
//				if(info_rx_len >= UART1_RECV_QUEUE_LENGTH)	en_info_rx = 0;
		}
					
		
		GPIO_Toggle(LED_PORT,LED_GREEN);

		

		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
	
	
	
	if(USART_GetFlagStatus(USART1,USART_FLAG_ORE)==SET)	  	//溢出处理
	{
		USART_ClearFlag(USART1,USART_FLAG_ORE);	            //清除ORE
		USART_ReceiveData(USART1);				            //读DR
	}	
}


void USART2_IRQHandler(void)
{
	uint8_t queue_read_buf;

	if(USART_GetITStatus(USART2,USART_IT_RXNE)==SET)   	//接收中断
	{
		queue_read_buf=USART_ReceiveData(USART2);		
//---------------------------------------------------------------------------
    USART_SendData(USART1, queue_read_buf);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
    {
    }

		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
	}
	
	
	
	if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)==SET)	  	//溢出处理
	{
		USART_ClearFlag(USART2,USART_FLAG_ORE);	            //清除ORE
		USART_ReceiveData(USART2);				            //读DR
	}	
}





void USART3_IRQHandler(void)
{
	uint8_t queue_read_buf;

	if(USART_GetITStatus(USART3,USART_IT_RXNE)==SET)   	//接收中断
	{
		queue_read_buf=USART_ReceiveData(USART3);		
		if(flag_usart3_rx==0)
		{
			UART3RecvBuf[usart3_rx_len]=queue_read_buf;
		usart3_rx_len++;
		
		if(usart3_rx_len>=UART3_RECV_QUEUE_LENGTH)
		{
			usart3_rx_len=0;
			flag_usart3_rx=1;
		}
		}
//---------------------------------------------------------------------------


		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
	}
	
	
	
	if(USART_GetFlagStatus(USART3,USART_FLAG_ORE)==SET)	  	//溢出处理
	{
		USART_ClearFlag(USART3,USART_FLAG_ORE);	            //清除ORE
		USART_ReceiveData(USART3);				            //读DR
	}	
}


void USB_LP_CAN1_RX0_IRQHandler(void)
{
  CAN_Receive(CAN1,CAN_FIFO0, &CANRxMessage);
	
//	CAN_ITConfig(CAN1, CAN_IT_FMP0, DISABLE);

}

/**
  * @brief  This function handles RTC global interrupt request.
  * @param  None
  * @retval : None
  */
void RTC_IRQHandler(void)
{
  if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
  {
    /* Clear the RTC Second interrupt */
    RTC_ClearITPendingBit(RTC_IT_SEC);

    /* Toggle GPIO_LED pin 6 each 1s */
    //GPIO_WriteBit(GPIO_LED, GPIO_Pin_6, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIO_LED, GPIO_Pin_6)));

    /* Enable time update */
    TimeDisplay = 1;

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    /* Reset RTC Counter when Time is 23:59:59 */
    if (RTC_GetCounter() == 0x00015180)
    {
      RTC_SetCounter(0x0);
      /* Wait until last write operation on RTC registers has finished */
      RTC_WaitForLastTask();
    }
  }
}


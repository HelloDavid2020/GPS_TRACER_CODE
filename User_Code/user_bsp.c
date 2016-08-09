#include "user_bsp.h"

uint8_t g_needReportAlarm = 0;

uint32_t ChipUniqueID[3];
RCC_ClocksTypeDef RCC_Clock;

void Get_ChipID(void)
{
	ChipUniqueID[0] = *(__IO u32 *)(0X1FFFF7F0); // 高字节
	ChipUniqueID[1] = *(__IO u32 *)(0X1FFFF7EC); // 
	ChipUniqueID[2] = *(__IO u32 *)(0X1FFFF7E8); // 低字节
}



void DisableInt(void)
{
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
	USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
	USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);

  CAN_ITConfig(CAN1, CAN_IT_FMP0, DISABLE);
	TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);
	TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);
}


void EnableInt(void)
{
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

  CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
}

/********************************************************************************************************
**函数信息 ：void BSP_Init(void)                         // WAN.CG // 2010.12.12 
**功能描述 ：硬件配置初始化函数，可初始化一些开机启动后急需配置的外设等
**输入参数 ：无
**输出参数 ：无
**调用提示 ：
********************************************************************************************************/
void BspInit(void)
{
	RCC_GetClocksFreq(&RCC_Clock);
	USART1_Init(115200);
	USART2_Init(115200);
	USART3_Init(9600);
	User_GpioInit();	
//	exit_wake_up();
	User_Timer2_Configuration();

	
	LED_SystemRun();
	
	printf("--------------------------------------------------\r\n");
	printf("> SYSCLK clock frequency   %dHz \r\n",RCC_Clock.SYSCLK_Frequency);
	printf("> HCLK   clock frequency   %dHz \r\n",RCC_Clock.HCLK_Frequency);
	printf("> PCLK1  clock frequency   %dHz \r\n",RCC_Clock.PCLK1_Frequency);
	printf("> PCLK2  clock frequency   %dHz \r\n",RCC_Clock.PCLK2_Frequency);
	printf("> ADCCLK clock frequency   %dHz \r\n",RCC_Clock.ADCCLK_Frequency);
	printf("--------------------------------------------------\r\n");
	
	LSM330DLC_Init();
	SPI1_Init();
	ADC_Configuration();
	sd_spi_init();
}

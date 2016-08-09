#include "user_dma.h"


void User_DMA_Init(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	/* Enable DMA1 clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_DeInit(DMA1_Channel6); 
	/* Periph addr is usart rx data register address  */
	DMA_InitStructure.DMA_PeripheralBaseAddr    = USART2_BASE+4;
	DMA_InitStructure.DMA_MemoryBaseAddr        = (uint32_t)gps_buffer;
	DMA_InitStructure.DMA_BufferSize            = NMEA_COUNT_MAX;

	/* Usart rx data reg need not increment, rx buffer nned */
	DMA_InitStructure.DMA_PeripheralInc         = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc             = DMA_MemoryInc_Enable;
	/* Once transfer one byte and using cycle mode  */
	DMA_InitStructure.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize        = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode                  = DMA_Mode_Circular;           
	/* Using highest priority, transfer direction is perihp to memory(USART to memory)  */    
	DMA_InitStructure.DMA_Priority              = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_DIR                   = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_M2M                   = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel6, &DMA_InitStructure);	
	
	/* Enable USART2 RX DMA channel */
	DMA_Cmd(DMA1_Channel6, ENABLE);

}



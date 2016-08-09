#include "spi_dma.h"


uint8_t spi1_tx_buf[6]={0xA8,0xA9,0xAA,0xAB,0xAC,0xAD};
uint8_t dma_rx_buf[12]={0x00};
uint8_t spi1_rx_buf[12]={0x00};

uint8_t spi1_tx_index=0;
uint8_t spi1_rx_index=0;
int16_t gZ= 0;
float f_gZ= 0;

void get_lms330_data(void)
{
	static uint8_t dma_tx_index  = 0;
	static uint8_t dma_rx_index = 0;
	
//	GPIO_SetBits(GPIOB, GPIO_Pin_8);
//	GPIO_ResetBits(GPIOA, GPIO_Pin_4); 
//	
//	GPIO_ResetBits(GPIOB, GPIO_Pin_8);
//	GPIO_SetBits(GPIOA, GPIO_Pin_4); 

	SPI_SendByte(spi1_tx_buf[dma_tx_index]);
	
			while (DMA_GetFlagStatus(DMA1_FLAG_TC2) == RESET);
			
				spi1_rx_buf[dma_tx_index] = dma_rx_buf[0];
			
	dma_tx_index++;
	
	
	
	if(dma_tx_index>5) dma_tx_index = 0;
//	
	  gZ = (int16_t)( (int16_t)spi1_rx_buf[1]<<8 | spi1_rx_buf[0] );
		f_gZ = gZ* GYRORATE;

}

		

void SPI1_DMA_Configuration( void )
{
  DMA_InitTypeDef DMA_InitStructure;
    /* DMA1 Channel2 (triggered by SPI1 Rx event) Config */
  DMA_DeInit(DMA1_Channel2);  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;                          //设置 SPI1 发送外设(0x4001300C) 地址(目的地址)
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)dma_rx_buf;                    //设置 SRAM 存储地址(目的地址)
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                                //传输方向 外设-内存
  DMA_InitStructure.DMA_BufferSize = 1;                         //设置 SPI1 发送长度
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; // DMA_Mode_Circular   DMA_Mode_Normal
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel2, &DMA_InitStructure);
  
  DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);
  /* Enable SPI1 DMA RX request */
  SPI1->CR2 |= 1<<0;                                                                 //接收缓冲区DMA使能
  DMA_Cmd(DMA1_Channel2, ENABLE);
    
    
    /* DMA1 Channel3 (triggered by SPI1 Tx event) Config */
  DMA_DeInit(DMA1_Channel3);  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;                          //设置  接收外设(0x4001300C) 地址(源地址)
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)spi1_tx_buf;                    //设置 SRAM 存储地址(源地址)
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                                //传输方向 内存-外设
  DMA_InitStructure.DMA_BufferSize = 1;                           //设置 SPI1 接收长度
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;                  //外设地址增量(不变)
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                           //内存地址增量(变化)
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;           //外设传输宽度(字节)
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;                   //内存传输宽度(字节)
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                                     //传输方式,一次传输完停止,不重新加载
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                           //中断方式-高(三级)
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                      //内存到内存方式禁止
  DMA_Init(DMA1_Channel3, &DMA_InitStructure);
  
  DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, DISABLE);                                   //开启 DMA1_Channel3 传输完成中断
  DMA_ITConfig(DMA1_Channel3, DMA_IT_TE, ENABLE);                                   //开启 DMA1_Channel3 传输错误中断
  /* Enable SPI1 DMA TX request */
  SPI1->CR2 |= 1<<1;                                                                //发送缓冲区DMA使能
  DMA_Cmd(DMA1_Channel3, DISABLE);                                                  //开启 DMA 通道 DMA1_Channel3
	




	
//    RCC->AHBENR |= 1<<0 ;                     //DMA1时钟使能

//	/*------------------配置SPI1_RX_DMA通道Channel2---------------------*/

//    DMA1_Channel2->CCR &= ~( 1<<14 ) ;        //非存储器到存储器模式
//	DMA1_Channel2->CCR |=    2<<12   ;        //通道优先级高
//	DMA1_Channel2->CCR &= ~( 3<<10 ) ;        //存储器数据宽度8bit
//	DMA1_Channel2->CCR &= ~( 3<<8  ) ;        //外设数据宽度8bit
//	DMA1_Channel2->CCR |=    1<<7    ;        //存储器地址增量模式
//	DMA1_Channel2->CCR &= ~( 1<<6  ) ;        //不执行外设地址增量模式
//	DMA1_Channel2->CCR &= ~( 1<<5  ) ;        //执行循环操作
//	DMA1_Channel2->CCR &= ~( 1<<4  ) ;        //从外设读

//	DMA1_Channel2->CNDTR &= 0x0000   ;        //传输数量寄存器清零
//	DMA1_Channel2->CNDTR = 1 ;       //传输数量设置为buffersize个

//	DMA1_Channel2->CPAR = (uint32_t)&SPI1->DR ;      //设置外设地址，注意PSIZE
//	DMA1_Channel2->CMAR = (uint32_t)spi1_rx_buf; //设置DMA存储器地址，注意MSIZE

//	/*------------------配置SPI1_TX_DMA通道Channel3---------------------*/

//	DMA1_Channel3->CCR &= ~( 1<<14 ) ;        //非存储器到存储器模式
//	DMA1_Channel3->CCR |=    0<<12   ;        //通道优先级最低
//	DMA1_Channel3->CCR &= ~( 3<<10 ) ;        //存储器数据宽度8bit
//	DMA1_Channel3->CCR &= ~( 3<<8 )  ;        //外设数据宽度8bit
//	DMA1_Channel3->CCR |=    1<<7    ;        //存储器地址增量模式
//	DMA1_Channel3->CCR &= ~( 1<<6 )  ;        //不执行外设地址增量模式
//	DMA1_Channel3->CCR &= ~( 1<<5 ) ;         //不执行循环操作
//	DMA1_Channel3->CCR |=    1<<4    ;        //从存储器读

//	DMA1_Channel3->CNDTR &= 0x0000   ;        //传输数量寄存器清零
//	DMA1_Channel3->CNDTR = 1 ;       //传输数量设置为buffersize个
//	
//	DMA1_Channel3->CPAR = (uint32_t)&SPI1->DR ;      //设置外设地址，注意PSIZE
//	DMA1_Channel3->CMAR = (uint32_t)spi1_tx_buf ; //设置DMA存储器地址，注意MSIZE				 
}




void SPI1_ReceiveSendByte( u16 num )
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);
	GPIO_SetBits(GPIOA, GPIO_Pin_4); 

	
	DMA1_Channel2->CNDTR = 0x0000   ;           //传输数量寄存器清零
	DMA1_Channel2->CNDTR = num ;         //传输数量设置为buffersize个

	DMA1_Channel3->CNDTR = 0x0000   ;           //传输数量寄存器清零
	DMA1_Channel3->CNDTR = num ;         //传输数量设置为buffersize个

	DMA1->IFCR = 0xF0 ;                         //清除通道2的标志位
	DMA1->IFCR = 0xF00 ;                        //清除通道3的标志位

	SPI1->DR ;									//接送前读一次SPI1->DR，保证接收缓冲区为空

	while( ( SPI1->SR & 0x02 ) == 0 );
	
	DMA1_Channel3->CCR |= 1 << 0 ;              //开启DMA通道3
	DMA1_Channel2->CCR |= 1 << 0 ;              //开启DMA通道2	

	while( ( DMA1->ISR & 0x20 ) == 0 );

	DMA1_Channel3->CCR &= ~( 1 << 0 ) ;         //关闭DMA通道3
	DMA1_Channel2->CCR &= ~( 1 << 0 ) ;         //关闭DMA通道2
	
	
  /* Set chip select High at the end of the transmission */ 
	GPIO_SetBits(GPIOB, GPIO_Pin_8);
	GPIO_SetBits(GPIOA, GPIO_Pin_4);	

}

//SPI发送一个字节
uint8 SPI_SendByte(uint8 Data)
{
	spi1_tx_buf[0] = Data;
	SPI1_ReceiveSendByte(1);

	return (spi1_rx_buf[0]);
}

////SPI读取Len字节
//void SPI_ReadBytes( uint8 Addr, NtrxBufferPtr Buffer, uint8 Len )
//{
//	uint8 i = 0;

//	if( ( Len > 0x80 ) || ( Len == 0 ) )
//		Len = 1 ;

//	SPI1_TX_Buff[0] = Len;
//	SPI1_TX_Buff[1] = Addr;

//	SPI1_ReceiveSendByte(Len + 2);

//	for (i = 0;i < Len;i++)
//	{
//	 	*Buffer++ = SPI1_RX_Buff[i + 2];	
//	}
//}

unsigned char SPI_Read(unsigned char* data,unsigned char bytesNumber)
{
    unsigned char byte = 0;
    for(byte = 0; byte < bytesNumber; byte++)
    {
         spi1_tx_buf[byte]=0;
    }
    SPI1_ReceiveSendByte(bytesNumber);
    for(byte = 0; byte < bytesNumber; byte++)
    {
         data[byte]=spi1_rx_buf[byte];
    }
    return bytesNumber;
}

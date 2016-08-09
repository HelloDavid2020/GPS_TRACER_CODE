#include "stm32f10x.h"
#include "spi.h"

//SPI 速度设置函数
//SpeedSet:
//SPI_BaudRatePrescaler_2   2分频   
//SPI_BaudRatePrescaler_8   8分频   
//SPI_BaudRatePrescaler_16  16分频  
//SPI_BaudRatePrescaler_256 256分频 
  
void SPI3_SetSpeed(uint8_t SpeedSet)
{
	SPI3->CR1&=0XFFC7; 
	SPI3->CR1|=SpeedSet;
	SPI_Cmd(SPI3,ENABLE); 
} 

//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
uint8_t SPI3_ReadWriteByte(u8 TxData)
{		
	uint8_t retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
	{
	retry++;
	if(retry>200)return 0;
	}			  
	SPI_I2S_SendData(SPI3, TxData); //通过外设SPIx发送一个数据
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
	{
	retry++;
	if(retry>200)return 0;
	}	  						    
	return SPI_I2S_ReceiveData(SPI3); //返回通过SPIx最近接收的数据					    
}

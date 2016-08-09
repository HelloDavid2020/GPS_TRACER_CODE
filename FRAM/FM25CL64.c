#include  "FM25CL64.h"
/******************** (C) COPYRIGHT ********************

*          ---------------------------------
*         | PB15-SPI2-MOSI  : FM25CL64-SI  |
*         | PB14-SPI2-MISO  : FM25CL64-SO  |
*         | PB13-SPI2-SCK   : FM25CL64-SCK |
*         | PB12-SPI2-NSS   : FM25CL64-CS  |
*          ---------------------------------
*******************************************************/

uint8_t FRAM_writeBuf[1024] = {0};
uint8_t FRAM_readBuf[1024] = {0};

uint32_t PassedCount = 0;
uint32_t FailedCount = 0;

uint8_t SPI1_WriteByte(uint8_t byte)
{
	uint8_t dummy = SPI1->DR; 		//Clear RXNE Flag
	while((SPI1->SR & SPI_I2S_FLAG_TXE) == 0);
	SPI1->DR = byte;
	while((SPI1->SR & SPI_I2S_FLAG_RXNE)==0);
	byte = SPI1->DR;
	return byte;
}




uint8_t read_fram_id(void)
{
	uint8_t res = 0;
		unsigned char id[10];
		unsigned char i,num;
//--------------------------------------------------   
		FRAM_CS_LOW();		
		__nop();
		__nop(); 
		__nop();
		__nop(); 
		
		FM25CL64_WRByte(0x9F);
		for(i=0;i<9;i++)
		{
			id[i] = FM25CL64_RDByte();
		}
		FRAM_CS_HIGH() ;									  
		__nop();
		__nop(); 
		
		if( id[6] == 0xC2 && id[7] == 0x23 && id[8] == 0x00)
		{			
			res= 1;
			printf("> FRAM Type: FM25V05 \r\n");
		}
		else if( id[6] == 0xC2 && id[7] == 0x22 && id[8] == 0x00)
		{
			res= 2;
			printf("> FRAM Type: FM25V02 \r\n");
		}
		else
		{
			res=0;
			printf("> FRAM Type: UNKONWN %02X,%02X,%02X\r\n",id[6],id[7],id[8]);
		}
		return res;

}


/*******************************************************************************
* Function Name  : SPI_FRAM_ReadByte
* Description    : Reads a byte from the SPI Flash.
*                  This function must be used only if the Start_Read_Sequence
*                  function has been previously called.
* Input          : None
* Output         : None
* Return         : Byte Read from the SPI Flash.
*******************************************************************************/
uint8_t SPI_FRAM_ReadByte(void)
{
	return (SPI_FRAM_SendByte(Dummy_Byte));
}



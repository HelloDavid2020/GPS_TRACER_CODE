
/******************************************************************************
*2010.11.26修改说明
*对外函数加读状态，上次操作未完成直接返回0
*BUFFER读写两函数加最大操作上限1024，超出直接返回0 
*等待函数加看门狗程序
*by wong
******************************************************************************/
#include "MX25_CMD.h"

#include "spi_flash.h"	 
#include "MX25_DEF.h"
//#include "CommonPeripheral.h"


unsigned long SPI_FLASH1_SIZE = 0x400000; 
unsigned long SPI_FLASH2_SIZE = 0x2000000;

extern void Wait_Flash_WarmUp(void);
/*
FLASH_CS	- 	PA8
*/
void SPI_FLASH_Init(void)
{
	unsigned char a = 0;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_SetBits(GPIOA, GPIO_Pin_8);
	//Init IO Port
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// Wait flash warm-up
  Wait_Flash_WarmUp();
	a = CMD_EN4B();
	if(a)
	{
		a = a;
	}
}

void CS_Low(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_7);
}

void CS_High(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_7);
}

void SendByte(uint8 byte_value, uint8 transfer_type)
{
	transfer_type = 0;
	SPI1_WriteByte(byte_value);
}

uint8 GetByte( uint8 transfer_type )
{
	return SPI1_WriteByte(transfer_type);
}

//void SPI_FLASH_ChipSelect(ENUM_FLASH flash_num,unsigned char en)
//{
//	if(flash_num == FLASH1)
//	{
//		if(en == 0)	
//		{
//			SPI_FLASH_CS1_HIGH();
//			SPI_FLASH_CS2_HIGH();
//		}
//		else
//		{						 
//			SPI_FLASH_CS1_LOW();
//			SPI_FLASH_CS2_LOW();
//		}
//	}
//}		 

//u8 SPI_FLASH_SendByte(u8 byte)
//{
//	while(SPI_I2S_GetFlagStatus(Flash_SPI, SPI_I2S_FLAG_TXE)  == RESET) ;
//	SPI_I2S_SendData(Flash_SPI, byte)                                   ;
//	while(SPI_I2S_GetFlagStatus(Flash_SPI, SPI_I2S_FLAG_RXNE) == RESET)	;
//	return SPI_I2S_ReceiveData(Flash_SPI)                               ;
//}

//void SPI_FLASH_WriteEnable(ENUM_FLASH flash_num)
//{
//	/* Select the FLASH: Chip Select low */
//	SPI_FLASH_ChipSelect(flash_num,1); 

//	SPI_FLASH_SendByte(FLASH_CMD_WREN);	

//	/* Deselect the FLASH: Chip Select high */
//	SPI_FLASH_ChipSelect(flash_num,0);		
//}
//void SPI_FLASH_WriteDIS(ENUM_FLASH flash_num)
//{
//	/* Select the FLASH: Chip Select low */
//	SPI_FLASH_ChipSelect(flash_num,1); 
//
//	SPI_FLASH_SendByte(4);	
//
//	/* Deselect the FLASH: Chip Select high */
//	SPI_FLASH_ChipSelect(flash_num,0);		
//}
//unsigned char Last_operation_check(ENUM_FLASH flash_num)
//{
//	unsigned char flashstatus = 0;
//	unsigned char state;

//	/* Select the FLASH: Chip Select low */
//	SPI_FLASH_ChipSelect(flash_num,1); 
//	/* Send "Read Status Register" instruction */
//	SPI_FLASH_SendByte(FLASH_CMD_RDSR);
//	state = 1;
//	flashstatus = SPI_FLASH_SendByte(0xff);
//	if(flashstatus & FLASH_WIP_FLAG) state = 0;
//	else state = 1;
//	/* Deselect the FLASH: Chip Select high */
//	SPI_FLASH_ChipSelect(flash_num,0);
//	return state;	
//}

//unsigned char SPI_FLASH_WaitForWriteEnd(ENUM_FLASH flash_num)
//{
//	unsigned char flashstatus = 0;
//	unsigned long tmp = 0;
//	unsigned char i = 0;
//	unsigned char state;

//	/* Select the FLASH: Chip Select low */
//	SPI_FLASH_ChipSelect(flash_num,1); 
//	/* Send "Read Status Register" instruction */
//	SPI_FLASH_SendByte(FLASH_CMD_RDSR);

//	state = 1;
//	/* Loop as long as the memory is busy with a write cycle */
//	do
//	{
//		/* Send a dummy byte to generate the clock needed by the FLASH
//		and put the value of the status register in FLASH_Status variable */
//		flashstatus = SPI_FLASH_SendByte(0xff);
//		if((xTaskGetTickCount()-tmp) >= 400)
//		{
//			feed_dog();
//			i++;  
//			tmp = xTaskGetTickCount(); 			
//		}	
//		if(i > 200)
//		{
//			state = 0;
//			break;
//		} 	
//	}
//	while ((flashstatus & FLASH_WIP_FLAG) == SET); /* Write in progress */

//	/* Deselect the FLASH: Chip Select high */
//	SPI_FLASH_ChipSelect(flash_num,0);
//	return state;
//}

/************************************************************************************
** SPI_FLASH_ReadID()查询FLASH存储器ID号
**					输入参数说明：要查询的FLASH的序号
**					返回值说明：返回查询FLASH的ID号
*************************************************************************************/
//unsigned long SPI_FLASH_ReadID(ENUM_FLASH flash_num)
//{	
//	unsigned long Temp = 0;
//	unsigned long Temp1 = 0;
//	unsigned long Temp2 = 0;
//	unsigned long Temp3 = 0;
//	
//	/* Select the FLASH: Chip Select low */
//	SPI_FLASH_ChipSelect(flash_num,1); 	   	
//	/* Send "RDID " instruction */
//	SPI_FLASH_SendByte(FLASH_CMD_RDID);	 	
//	/* Read a byte from the FLASH */
//	Temp1 = SPI_FLASH_SendByte(0xff);		
//	/* Read a byte from the FLASH */
//	Temp2 = SPI_FLASH_SendByte(0xff);		
//	/* Read a byte from the FLASH */
//	Temp3 = SPI_FLASH_SendByte(0xff);		
//	/* Deselect the FLASH: Chip Select high */
//	SPI_FLASH_ChipSelect(flash_num,0); 
//	
//	Temp = (Temp1 << 16) | (Temp2 << 8) | Temp3;
//	
//	return Temp;
//}														 

/*******************************************************************************
* Function Name  : SPI_FLASH_4KErase
* Description    : Erase sector
* Input          : sector address
* Output         : None
* Return         : None
*******************************************************************************/
unsigned char SPI_FLASH_4KErase(ENUM_FLASH flash_num,unsigned long SectorAddr)
{
	if (flash_num == FLASH1)
	{
		if (CMD_SE(SectorAddr) == FlashOperationSuccess)
		{
			return 1;
		}
	}
	return 0;
}

/*******************************************************************************
* Function Name  : SPI_FLASH_SectorErase
* Description    : Erase sector
* Input          : sector address
* Output         : None
* Return         : None
*******************************************************************************/
unsigned char SPI_FLASH_64KErase(ENUM_FLASH flash_num,unsigned long SectorAddr)
{
	if (flash_num == FLASH1)
	{
		if (CMD_BE(SectorAddr) == FlashOperationSuccess)
		{
			return 1;
		}
	}
	return 0;
}


/*******************************************************************************
* Function Name  : SPI_FLASH_BulkErase
* Description    : Erase entire FLASH
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
unsigned char SPI_FLASH_BulkErase(ENUM_FLASH flash_num)
{
	if (flash_num == FLASH1)
	{
		if (CMD_CE() == FlashOperationSuccess)
		{
			return 1;
		}
	}
	return 0;
}

/*******************************************************************************
* Function Name  : SPI_FLASH_PageWrite
* Description    : Write page 
* Input          : Buffer to write,target address,number of bytes to write
* Output         : None
* Return         : None
*******************************************************************************/
//unsigned char SPI_FLASH_PageWrite(ENUM_FLASH flash_num,unsigned char* pBuffer,unsigned long WriteAddr,unsigned short NumByteToWrite)
//{
//	/* Enable the write access to the FLASH */
//	SPI_FLASH_WriteEnable(flash_num);

//	/* Select the FLASH: Chip Select low */
//	SPI_FLASH_ChipSelect(flash_num,1);
//	/* Send "Write to Memory " instruction */
//	SPI_FLASH_SendByte(FLASH_CMD_WRITE);
//	/* Send WriteAddr high nibble address byte to write to */
//	SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
//	/* Send WriteAddr medium nibble address byte to write to */
//	SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
//	/* Send WriteAddr low nibble address byte to write to */
//	SPI_FLASH_SendByte(WriteAddr & 0xFF);

//	/* while there is data to be written on the FLASH */
//	while (NumByteToWrite--)
//	{
//		/* Send the current byte */
//		SPI_FLASH_SendByte(*pBuffer);
//		/* Point on the next byte to be written */
//		pBuffer++;
//	} 
//	/* Deselect the FLASH: Chip Select high */
//	SPI_FLASH_ChipSelect(flash_num,0);
//	/* Wait the end of Flash writing */
//	if(SPI_FLASH_WaitForWriteEnd(flash_num) == 0)	return 0;
//	else	return 1;
//}  

//void SPI_FLASH_RandomRead(ENUM_FLASH flash_num,unsigned char* pBuffer,unsigned long ReadAddr,unsigned short NumByteToRead)
//{
//	/* Select the FLASH: Chip Select low */
//	SPI_FLASH_ChipSelect(flash_num,1);	
//	/* Send "Read from Memory " instruction */
//	SPI_FLASH_SendByte(FLASH_CMD_READ);	
//	/* Send ReadAddr high nibble address byte to read from */
//	SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
//	/* Send ReadAddr medium nibble address byte to read from */
//	SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);  
//	/* Send ReadAddr low nibble address byte to read from */
//	SPI_FLASH_SendByte(ReadAddr & 0xFF);
//	while(NumByteToRead--) /* while there is data to be read */
//	{
//		/* Read a byte from the FLASH */
//		*pBuffer = SPI_FLASH_SendByte(0xff);
//		/* Point to the next location where the byte read will be saved */
//		pBuffer++;
//	}
//	/* Deselect the FLASH: Chip Select high */
//	SPI_FLASH_ChipSelect(flash_num,0);
//}

/*******************************************************************************
* Function Name  : SPI_FLASH_BufferWrite
* Description    : Write buffer
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
unsigned char SPI_FLASH_BufferWrite(ENUM_FLASH flash_num,unsigned char* pBuffer,unsigned long WriteAddr,unsigned short NumByteToWrite)
{
	int max_write=0;
	if (flash_num == FLASH1)
	{
		max_write = 0x100 - (WriteAddr & 0xFF);
		while(NumByteToWrite > 0)
		{
			if (NumByteToWrite > max_write)
			{
				if (CMD_PP(WriteAddr, pBuffer, max_write) != FlashOperationSuccess)
					return 0;
				WriteAddr += max_write;
				pBuffer += max_write;
				NumByteToWrite -= max_write;
			}
			else
			{
				if (CMD_PP(WriteAddr, pBuffer, NumByteToWrite) != FlashOperationSuccess)
					return 0;
				WriteAddr += NumByteToWrite;
				pBuffer += NumByteToWrite;
				NumByteToWrite = 0;
			}
			max_write = 0x100;
		}
		return 1;
	}
	return 0;
}


/*******************************************************************************
* Function Name  : SPI_FLASH_ReadAddr
* Description    : Set the address to read
* Input          : - ReadAddr : FLASH's internal address to read from.
* Output         : None
* Return         : None
*******************************************************************************/
//void SPI_FLASH_ReadAddr(ENUM_FLASH flash_num,unsigned long ReadAddr)
//{
//	/* Select the FLASH: Chip Select low */
//	SPI_FLASH_ChipSelect(flash_num,1);	   
//	
//	/* Send "Read from Memory " instruction */
//	SPI_FLASH_SendByte(FLASH_CMD_READ);	 
//	
//	/* Send ReadAddr high nibble address byte to read from */
//	SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
//	/* Send ReadAddr medium nibble address byte to read from */
//	SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);  
//	/* Send ReadAddr low nibble address byte to read from */
//	SPI_FLASH_SendByte(ReadAddr & 0xFF);
//}

/*******************************************************************************
* Function Name  : SPI_FLASH_BufferRead
* Description    : Read buffer
* Input          : Buffer address number
* Output         : None
* Return         : None
*******************************************************************************/
unsigned char SPI_FLASH_BufferRead(ENUM_FLASH flash_num,unsigned char* pBuffer,unsigned long ReadAddr,unsigned short NumByteToRead)
{
	int max_read;
	if (flash_num == FLASH1)
	{
		max_read = 0x100 - (ReadAddr & 0xFF);
		while (NumByteToRead>0)
		{
			if (NumByteToRead > max_read)
			{
				if (CMD_READ(ReadAddr, pBuffer, max_read) != FlashOperationSuccess)
					return 0;
				ReadAddr += max_read;
				pBuffer += max_read;
				NumByteToRead -= max_read;
			}
			else
			{
				if (CMD_READ(ReadAddr, pBuffer, NumByteToRead) != FlashOperationSuccess)
					return 0;
				ReadAddr += NumByteToRead;
				pBuffer += NumByteToRead;
				NumByteToRead = 0;
			}
			max_read = 0x100;
		}
		return 1;
	}
	return 0;
}

/************************************************************************************
** SPI_FLASH_CheckCap()查询FLASH存储器容量
**					输入参数说明：要查询的FLASH的序号
**					返回值说明：返回查询FLASH的容量
*************************************************************************************/
//unsigned long SPI_FLASH_CheckCap(ENUM_FLASH flash_num)
//{
//	unsigned char mem_id = 0;
//	unsigned long flash_id = 0;
//	unsigned long mem_cap = 0;

//	flash_id = SPI_FLASH_ReadID(flash_num);
//	mem_id = flash_id%0x1000000;
//	if(mem_id == 0x13)	
//		mem_cap = 0x80000;
//	else if(mem_id == 0x14)	
//		mem_cap = 0x100000;
//	else if(mem_id == 0x15)
//			mem_cap = 0x200000;
//	else if(mem_id == 0x16)	
//		mem_cap = 0x400000;
//	else if(mem_id == 0x17) 
//		mem_cap = 0x800000;
//	else if(mem_id == 0x18) 
//		mem_cap = 0x1000000;
//	else if(mem_id == 0x19) 
//		mem_cap = 0x2000000;
//	else 
//		return 0;
//	
//	return mem_cap;		
//}





